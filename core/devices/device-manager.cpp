#include <core/containers/device-info-container.h>
#include <core/devices/device-manager.h>
#include <core/devices/device-port.h>
#include <core/devices/device-thread.h>
#include <core/factories/emitter-factory.h>

#include <memory>

using namespace Device;

DeviceManager::DeviceManager(QObject *parent)
	: QObject(parent)

{
	connect(&m_deviceScan, &QTimer::timeout, this, &DeviceManager::rescan);
	m_deviceScan.setInterval(2000);
	m_deviceScan.start();
}

DeviceManager::~DeviceManager() {
}

void DeviceManager::rescan() {
	Container::DeviceInfoContainer deviceInfo("LedFrame", "LedFrame", 500000);
	const QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

	for (int i = 0; i < ports.count(); ++i) {
		if ((ports[i].manufacturer() != deviceInfo.manufacturer())) continue;

		auto device = std::make_unique<DevicePort>(ports[i]);

		if (!device->open(QIODevice::ReadWrite))
			return;

		device->setBaudRate(deviceInfo.baudrate());
		device->setFlowControl(QSerialPort::NoFlowControl);
		device->setParity(QSerialPort::NoParity);
		device->setDataBits(QSerialPort::Data8);
		device->setStopBits(QSerialPort::OneStop);
		auto thread = std::make_unique<DeviceThread>(std::move(device), ports[i]);
		connect(thread.get(), &DeviceThread::finished, this, &DeviceManager::remove, Qt::QueuedConnection);

		if (m_registerDeviceCallback && !m_registerDeviceCallback(thread.get(), ports[i].serialNumber()))
			continue;

		thread->connectEmitter(Factory::EmitterFactory::create(Enum::EmitterType::Animation));
		attach(std::move(thread));
	}
}

void DeviceManager::setRegisterDeviceCallback(const std::function<bool (Interface::IReceiver *, const QString &serialNumber)> &callback) {
	m_registerDeviceCallback = callback;
}

void DeviceManager::remove() {
	auto receiver = reinterpret_cast<Interface::IReceiver *>(sender());
	detach(receiver);
}

