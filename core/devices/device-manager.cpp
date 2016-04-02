#include <core/containers/device-info-container.h>
#include <core/devices/device-manager.h>
#include <core/devices/device-port.h>
#include <core/devices/device-thread.h>
#include <core/factories/emitter-factory.h>

#include <memory>

using namespace Device;

DeviceManager::DeviceManager(QObject *parent)
	: QObject(parent),
	  m_timerId(startTimer(250)) {
}

DeviceManager::~DeviceManager() {
	for (auto &thread : m_threads) {
		disconnect(thread.get(), 0, 0, 0);
		thread->interrupt();
		thread->wait();
	}
}

void DeviceManager::timerEvent(QTimerEvent *event) {
	Q_UNUSED(event);
	Container::DeviceInfoContainer deviceInfo("LedFrame", "LedFrame", 500000);
	QList <QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

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
		connect(thread.get(), &DeviceThread::finished, [this] {
			m_threads.remove_if([this](const std::unique_ptr<DeviceThread> &thread) {
				return thread.get() == sender();
			});
		});
		thread->start();
		thread->connectEmitter(Factory::EmitterFactory::create(Enum::EmitterType::Screen));
		m_threads.push_back(std::move(thread));
	}
}
