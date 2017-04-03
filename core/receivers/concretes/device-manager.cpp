#include <memory>

#include <core/containers/device-info-container.h>
#include <core/devices/device-port.h>
#include <core/networking/udp-broadcast-service.h>
#include <core/receivers/concretes/device-manager.h>
#include <core/receivers/concretes/uart-receiver.h>

using namespace Functional;
using namespace Network;
using namespace Receiver::Concrete;

ReceiverManager::ReceiverManager() {
	connect(&m_deviceScan, &QTimer::timeout, this, &ReceiverManager::rescan);
}

ReceiverManager::~ReceiverManager() {}

void ReceiverManager::rescan() {
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
			m_broadcasts.remove_if([&interface](const auto &match) {
				return interface->id() == match->id();
			});
			detach(interface);
		},
			Qt::QueuedConnection);

		if (m_registerDeviceCallback && !m_registerDeviceCallback(thread.get(), ports[i].serialNumber()))
			continue;

		m_broadcasts.emplace_back(std::make_unique<UdpBroadcastService>(thread->id(), thread->name(), 4999));
		attach(std::move(thread));
		emit afterAttach();
	}
}

void ReceiverManager::setRegisterDeviceCallback(const std::function<bool(Receiver::Interface::IReceiver *, const QString &serialNumber)> &callback) {
	m_registerDeviceCallback = callback;
}

void ReceiverManager::run() {
	m_deviceScan.setInterval(3000);
	m_deviceScan.start();
	rescan();
}
