#include <connector/alc-device-thread.h>
#include <core/containers/device-info-container.h>
#include <managers/alc-device-manager.h>
#include <core/factories/emitter-factory.h>

#include <memory>

ALCDeviceManager::ALCDeviceManager(QObject *parent)
	: QObject(parent),
	  m_timerId(startTimer(250)) {
}

ALCDeviceManager::~ALCDeviceManager() {
}

void ALCDeviceManager::timerEvent(QTimerEvent *event) {
	Q_UNUSED(event);
	Container::DeviceInfoContainer deviceInfo
	("LedFrame", "LedFrame", 500000);
	QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

	for (int i = 0; i < ports.count(); ++i) {
		if ((ports[i].manufacturer() != deviceInfo.manufacturer())) continue;

		auto device = std::make_unique<QSerialPort>(ports[i].portName());

		if (!device->open(QIODevice::ReadWrite))
			return;

		device->setBaudRate(deviceInfo.baudrate());
		device->setFlowControl(QSerialPort::NoFlowControl);
		device->setParity(QSerialPort::NoParity);
		device->setDataBits(QSerialPort::Data8);
		device->setStopBits(QSerialPort::OneStop);
		auto thread = std::make_unique<AmbientDeviceThread>(std::move(device), ports[i]);
		connect(thread.get(), &AmbientDeviceThread::finished, [this] {
			m_deviceThreads.remove_if([this](const std::unique_ptr<AmbientDeviceThread> &thread) {
				return thread.get() == sender();
			});
		});
		thread->start();
		thread->connectEmitter(Factory::EmitterFactory::create(Enum::EmitterType::Animation));
		m_deviceThreads.push_back(std::move(thread));
	}
}
