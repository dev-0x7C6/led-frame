#include "main-manager.h"

#include <core/containers/application-info-container.h>
#include <core/containers/device-info-container.h>
#include <core/correctors/factories/corrector-factory.h>
#include <core/devices/device-port.h>
#include <core/functionals/debug-notification.h>
#include <core/networking/udp-broadcast-service.h>
#include <core/receivers/concretes/uart-receiver.h>

#include <QSettings>

#include <mutex>

using namespace Enum;
using namespace Factory;
using namespace Functional;
using namespace Manager;
using namespace Network;
using namespace Receiver::Concrete;

MainManager::MainManager(QSettings &settings)
		: m_settings(settings)
		, m_globalBrightnessCorrection(make_corrector(CorrectorType::Brightness, -1))
		, m_globalRedCorrection(make_corrector(CorrectorType::RedChannel, -1))
		, m_globalGreenCorrection(make_corrector(CorrectorType::GreenChannel, -1))
		, m_globalBlueCorrection(make_corrector(CorrectorType::BlueChannel, -1)) {
	m_atoms.attach(m_globalBrightnessCorrection);
	m_atoms.attach(m_globalRedCorrection);
	m_atoms.attach(m_globalGreenCorrection);
	m_atoms.attach(m_globalBlueCorrection);

	m_settings.beginGroup("GlobalCorrectors");
	m_globalBrightnessCorrection->setFactor(m_settings.value("brightness", m_globalBrightnessCorrection->factor().value()).toUInt());
	m_globalRedCorrection->setFactor(m_settings.value("red", m_globalRedCorrection->factor().value()).toUInt());
	m_globalGreenCorrection->setFactor(m_settings.value("green", m_globalGreenCorrection->factor().value()).toUInt());
	m_globalBlueCorrection->setFactor(m_settings.value("blue", m_globalBlueCorrection->factor().value()).toUInt());
	m_settings.endGroup();

#ifdef QT_DEBUG
	attach(Functional::DebugNotification::instance());
#endif
	connect(&m_deviceScan, &QTimer::timeout, this, &MainManager::rescan);
}

MainManager::~MainManager() {
	m_settings.beginGroup("GlobalCorrectors");
	m_settings.setValue("brightness", m_globalBrightnessCorrection->factor().value());
	m_settings.setValue("red", m_globalRedCorrection->factor().value());
	m_settings.setValue("green", m_globalGreenCorrection->factor().value());
	m_settings.setValue("blue", m_globalBlueCorrection->factor().value());
	m_settings.endGroup();
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
	Container::DeviceInfo deviceInfo("LedFrame", "LedFrame", 500000);

	m_unregisterQueue.dequeue_all([this](auto &&id_to_unregister) {
		m_broadcasts.remove_if([id_to_unregister](auto &&match) { return id_to_unregister == match->id(); });
		m_atoms.detach(id_to_unregister);
	});

	for (auto &&port : QSerialPortInfo::availablePorts()) {
		if ((port.manufacturer().toStdString() != deviceInfo.manufacturer())) continue;

		if (!m_deviceLocker.lock(port.portName().toStdString()))
			continue;

		auto thread = std::make_unique<UartReceiver>(std::make_unique<DevicePort>(port),
			[this, port{port.portName().toStdString()}](const IRepresentable &value) {
				m_unregisterQueue.emplace(value.id());
				m_deviceLocker.unlock(port);
			});

		if (m_registerDeviceCallback && !m_registerDeviceCallback(thread.get(), port.serialNumber()))
			continue;

		m_broadcasts.emplace_back(std::make_unique<UdpBroadcastService>(thread->id(), thread->name(), 4999));
		m_atoms.attach(std::move(thread));
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
