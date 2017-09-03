#include "main-manager.h"

#include <core/containers/application-info-container.h>
#include <core/containers/device-info-container.h>
#include <core/correctors/factories/corrector-factory.h>
#include <core/devices/device-port.h>
#include <core/functionals/debug-notification.h>
#include <core/functionals/debug-notification.h>
#include <core/networking/udp-broadcast-service.h>
#include <core/receivers/concretes/uart-receiver.h>

#include <QSettings>

using namespace Corrector::Factory;
using namespace Enum;
using namespace Functional;
using namespace Manager;
using namespace Network;
using namespace Receiver::Concrete;

MainManager::MainManager(QSettings &settings)
		: m_settings(settings)
		, m_globalBrightnessCorrection(CorrectorFactory::create(CorrectorType::Brightness, -1))
		, m_globalRedCorrection(CorrectorFactory::create(CorrectorType::RedChannel, -1))
		, m_globalGreenCorrection(CorrectorFactory::create(CorrectorType::GreenChannel, -1))
		, m_globalBlueCorrection(CorrectorFactory::create(CorrectorType::BlueChannel, -1)) {
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

void MainManager::attach(Interface::IMultiNotifier &notifier) {
	m_atoms.attach(&notifier);
	m_atoms.enumerate([&notifier](const auto &atom) {
		if (Category::Receiver != atom->category())
			return;

		std::static_pointer_cast<Receiver::Interface::IReceiver>(atom)->correctors().attach(&notifier);
	});
}

void MainManager::detach(Interface::IMultiNotifier &notifier) {
	m_atoms.detach(&notifier);

	m_atoms.enumerate([&notifier](const auto &atom) {
		if (Category::Receiver != atom->category())
			return;

		std::static_pointer_cast<Receiver::Interface::IReceiver>(atom)->correctors().detach(&notifier);
	});
}

void MainManager::rescan() {
	Container::DeviceInfo deviceInfo("LedFrame", "LedFrame", 500000);
	const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

	static auto id = 0;

	for (int i = 0; i < ports.count(); ++i) {
		if ((ports[i].manufacturer() != deviceInfo.manufacturer())) continue;

		auto device = std::make_unique<DevicePort>(ports[i]);

		if (!device->open(QIODevice::ReadWrite))
			continue;

		device->setBaudRate(deviceInfo.baudrate());
		device->setFlowControl(QSerialPort::NoFlowControl);
		device->setParity(QSerialPort::NoParity);
		device->setDataBits(QSerialPort::Data8);
		device->setStopBits(QSerialPort::OneStop);
		auto thread = std::make_unique<UartReceiver>(id++, std::move(device));
		auto interface = thread.get();
		connect(interface, &UartReceiver::finished, this, [this, interface]() {
			m_broadcasts.remove_if([id = interface->id()](const auto &match) {
				return id == match->id();
			});
			m_atoms.detach(m_atoms.find(Category::Receiver, id));
		},
			Qt::QueuedConnection);

		if (m_registerDeviceCallback && !m_registerDeviceCallback(thread.get(), ports[i].serialNumber()))
			continue;

		m_broadcasts.emplace_back(std::make_unique<UdpBroadcastService>(thread->id(), thread->name(), 4999));
		m_atoms.attach(std::move(thread));
	}
}

void MainManager::setRegisterDeviceCallback(const std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> &callback) {
	m_registerDeviceCallback = callback;
}

void MainManager::run() {
	m_deviceScan.setInterval(3000);
	m_deviceScan.start();
	rescan();
}
