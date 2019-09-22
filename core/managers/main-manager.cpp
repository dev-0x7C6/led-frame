#include "main-manager.h"

#include <core/containers/application-info-container.h>
#include <core/containers/device-info-container.h>
#include <core/correctors/factories/corrector-factory.h>
#include <core/devices/device-port.h>
#include <core/functionals/debug-notification.h>
#include <core/networking/udp-broadcast-service.h>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/receivers/factories/receiver-factory.h>
#include <externals/common/logger/logger.hpp>

#include <QSettings>

using namespace Enum;
using namespace Factory;
using namespace Functional;
using namespace Manager;
using namespace Network;
using namespace Receiver::Concrete;

namespace {
constexpr auto filter = error_class::debug;
constexpr auto module = "[settings]: ";
constexpr auto serial_port_config_file = "/etc/led-frame/serial.conf";
} // namespace

class settings_group_raii {
public:
	settings_group_raii(QSettings &settings, const QString &key)
			: m_settings(settings) {
		m_settings.beginGroup(key);
	}

	~settings_group_raii() {
		m_settings.endGroup();
	}

private:
	QSettings &m_settings;
};

class SystemSerialPortConfiguration {
public:
	SystemSerialPortConfiguration() {
		QSettings settings(serial_port_config_file, QSettings::Format::IniFormat);
		if (QSettings::Status::NoError != settings.status()) {
			logger<filter>::warning(module, "unable to find or access serial port configuration file ", serial_port_config_file);
			logger<filter>::warning(module, "all ports allowed");
			return;
		}

		if (settings.childGroups().isEmpty()) {
			logger<filter>::warning(module, "no serial port configured: ", serial_port_config_file);
			logger<filter>::warning(module, "all ports allowed");
			return;
		}

		for (auto &&key : settings.childGroups()) {
			settings_group_raii raii(settings, key);
			auto port = settings.value("port").toString().toStdString();
			logger<filter>::debug(module, "section: ", key.toStdString());
			logger<filter>::debug(module, "port_name: ", port);
			m_ports.emplace_back(std::move(port));
		}
	}

	bool isRegistred(const std::string &device) {
		if (m_ports.empty())
			return true;

		return std::find(m_ports.begin(), m_ports.end(), device) != m_ports.end();
	}

private:
	std::vector<std::string> m_ports;
};

MainManager::MainManager(QSettings &settings)
		: m_settings(settings)
		, m_serialConfig(std::make_unique<SystemSerialPortConfiguration>())
		, m_globalBrightnessCorrection(make_corrector(CorrectorType::Brightness, -1))
		, m_globalRedCorrection(make_corrector(CorrectorType::RedChannel, -1))
		, m_globalGreenCorrection(make_corrector(CorrectorType::GreenChannel, -1))
		, m_globalBlueCorrection(make_corrector(CorrectorType::BlueChannel, -1)) {
	m_atoms.attach(m_globalBrightnessCorrection);
	m_atoms.attach(m_globalRedCorrection);
	m_atoms.attach(m_globalGreenCorrection);
	m_atoms.attach(m_globalBlueCorrection);

	{
		settings_group_raii raii(m_settings, "GlobalCorrectors");
		m_globalBrightnessCorrection->setFactor(m_settings.value("brightness", m_globalBrightnessCorrection->factor().value()).toUInt());
		m_globalRedCorrection->setFactor(m_settings.value("red", m_globalRedCorrection->factor().value()).toUInt());
		m_globalGreenCorrection->setFactor(m_settings.value("green", m_globalGreenCorrection->factor().value()).toUInt());
		m_globalBlueCorrection->setFactor(m_settings.value("blue", m_globalBlueCorrection->factor().value()).toUInt());
	}

#ifdef QT_DEBUG
	attach(Functional::DebugNotification::instance());
	auto receiver = factory::make_receiver(receiver_type::stub, [](auto &&) {});
	m_broadcasts.emplace_back(std::make_unique<UdpBroadcastService>(receiver->id(), "stub", 4999));
	m_atoms.attach(std::move(receiver));
#endif
	connect(&m_deviceScan, &QTimer::timeout, this, &MainManager::rescan);
}

MainManager::~MainManager() {
	settings_group_raii raii(m_settings, "GlobalCorrectors");
	m_settings.setValue("brightness", m_globalBrightnessCorrection->factor().value());
	m_settings.setValue("red", m_globalRedCorrection->factor().value());
	m_settings.setValue("green", m_globalGreenCorrection->factor().value());
	m_settings.setValue("blue", m_globalBlueCorrection->factor().value());
}

void MainManager::attach(INotification &notifier) noexcept {
	m_atoms.attach(&notifier);
	m_atoms.enumerate([&notifier](const auto &atom) {
		if (Category::Receiver != atom->category())
			return;

		std::static_pointer_cast<IReceiver>(atom)->correctors().attach(&notifier);
	});
}

void MainManager::detach(INotification &notifier) noexcept {
	m_atoms.detach(&notifier);
	m_atoms.enumerate([&notifier](const auto &atom) {
		if (Category::Receiver != atom->category())
			return;

		std::static_pointer_cast<IReceiver>(atom)->correctors().detach(&notifier);
	});
}

void MainManager::rescan() {
	m_unregisterQueue.dequeue_all([this](auto &&id_to_unregister) {
		m_broadcasts.remove_if([id_to_unregister](auto &&match) { return id_to_unregister == match->id(); });
		m_atoms.detach(id_to_unregister);
	});

	for (auto &&port : QSerialPortInfo::availablePorts()) {
		const auto portName = port.portName().toStdString();

		if (!m_serialConfig->isRegistred(portName))
			continue;

		if (!m_deviceLocker.lock(portName))
			continue;

		auto receiver = factory::make_receiver(
			receiver_type::uart, [this, port{port.portName().toStdString()}](const IRepresentable &value) {
				m_unregisterQueue.emplace(value.id());
				m_deviceLocker.unlock(port);
			},
			port);

		if (m_registerDeviceCallback && !m_registerDeviceCallback(receiver.get(), port.serialNumber()))
			continue;

		m_broadcasts.emplace_back(std::make_unique<UdpBroadcastService>(receiver->id(), port.portName().toStdString(), 4999));
		m_atoms.attach(std::move(receiver));
	}
}

void MainManager::setRegisterDeviceCallback(const std::function<bool(IReceiver *, const QString &serialNumber)> &callback) {
	m_registerDeviceCallback = callback;
}

void MainManager::run() {
	m_deviceScan.setInterval(3000);
	m_deviceScan.start();
	rescan();
}
