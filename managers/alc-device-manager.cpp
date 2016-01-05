#include <connector/alc-device-thread.h>
#include <core/containers/ambient-device-info-container.h>
#include <managers/alc-device-manager.h>

#include <memory>

ALCDeviceManager::ALCDeviceManager(QObject *parent)
	: QObject(parent),
		m_timerId(startTimer(250)) {
}

ALCDeviceManager::~ALCDeviceManager() {
}

void ALCDeviceManager::timerEvent(QTimerEvent *event) {
	Q_UNUSED(event);
	Container::AmbientDeviceInfoContainer deviceInfo
	("Ambient Led Connector", "ALC", 500000);
	QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

	for (int i = 0; i < ports.count(); ++i) {
		if ((ports[i].manufacturer() != deviceInfo.manufacturer()) ||
				(ports[i].description() !=  deviceInfo.decription())) continue;

		auto device = std::make_unique<QSerialPort>(ports[i].portName());

		if (!device->open(QIODevice::ReadWrite))
			return;

		device->setBaudRate(deviceInfo.baudrate());
		device->setFlowControl(QSerialPort::NoFlowControl);
		device->setParity(QSerialPort::NoParity);
		device->setDataBits(QSerialPort::Data8);
		device->setStopBits(QSerialPort::OneStop);
		auto thread = std::make_unique<ALCDeviceThread>(std::move(device), ports[i]);
		connect(thread.get(), &ALCDeviceThread::finished, [this] {
			m_deviceThreads.remove_if([this](const std::unique_ptr<ALCDeviceThread> &thread) {
				return thread.get() == sender();
			});
		});
		thread->start();
		m_deviceThreads.push_back(std::move(thread));
	}
}
