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
#include <externals/common/qt/raii/raii-settings-group.hpp>

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
			raii_settings_group raii(settings, key);
			auto port = settings.value("port").toString().toStdString();
			logger<filter>::debug(module, "section: ", key.toStdString());
			logger<filter>::debug(module, "port_name: ", port);
			m_ports.emplace_back(std::move(port));
		}
	}

	bool isAllowed(const std::string &device) {
		if (m_ports.empty())
			return true;

		return std::find(m_ports.begin(), m_ports.end(), device) != m_ports.end();
	}

private:
	std::vector<std::string> m_ports;
};

MainManager::MainManager(QSettings &settings)
		: m_settings(settings)
		, m_serialConfig(std::make_unique<SystemSerialPortConfiguration>()) {
	raii_settings_group _(m_settings, "global_correctors");
	for (auto &&type : {CorrectorType::Brightness, CorrectorType::RedChannel, CorrectorType::BlueChannel, CorrectorType::GreenChannel}) {
		std::shared_ptr corrector = make_corrector(type, -1);
		raii_settings_group _(m_settings, value(type));
		corrector->load(m_settings);
		m_global_correctors.emplace_back(corrector);
		m_atoms.attach(corrector);
	}

#ifdef QT_DEBUG
	attach(Functional::DebugNotification::instance());
	auto receiver = Factory::make_receiver(receiver_type::stub, [](auto &&) {});
	m_broadcasts.emplace_back(std::make_unique<UdpBroadcastService>(receiver->id(), "stub", 4999));
	m_atoms.attach(std::move(receiver));
#endif
	connect(&m_deviceScan, &QTimer::timeout, this, &MainManager::rescan);
}

MainManager::~MainManager() {
	raii_settings_group _(m_settings, "global_correctors");
	for (auto &&corrector : m_global_correctors) {
		raii_settings_group _(m_settings, value(corrector->type()));
		corrector->save(m_settings);
	}
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

		if (!m_serialConfig->isAllowed(portName))
			continue;

		if (!m_deviceLocker.lock(portName))
			continue;

		auto receiver = Factory::make_receiver(
			receiver_type::uart, [this, port{port.portName().toStdString()}](const IRepresentable &value) {
				m_unregisterQueue.emplace(value.id());
				m_deviceLocker.unlock(port);
			},
			port);

		if (!receiver) {
			m_deviceLocker.unlock(portName);
			continue;
		}

		if (m_registerDeviceCallback && !m_registerDeviceCallback(*receiver))
			continue;

		m_broadcasts.emplace_back(std::make_unique<UdpBroadcastService>(receiver->id(), receiver->name(), 4999));
		m_atoms.attach(std::move(receiver));
	}
}

void MainManager::setRegisterDeviceCallback(const std::function<bool(IReceiver &)> &callback) {
	m_registerDeviceCallback = callback;
}

void MainManager::run() {
	m_deviceScan.setInterval(3000);
	m_deviceScan.start();
	rescan();
}
