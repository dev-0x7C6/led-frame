#include <core/containers/device-config-container.h>
#include <core/devices/device-port.h>
#include <core/devices/device-thread.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-stream.h>
#include <core/functionals/loop-sync.h>
#include <core/correctors/color-enhancer-corrector.h>

#include <QElapsedTimer>
#include <algorithm>
#include <memory>

using namespace Container;
using namespace Device;
using namespace Enum;

DeviceThread::DeviceThread(std::unique_ptr<DevicePort> &&device, QSerialPortInfo details, QObject *parent)
	: QThread(parent),
	  Abstract::AbstractReceiver(),
	  m_device(std::move(device)),
	  m_details(details),
	  m_interrupt(false) {
	m_device->moveToThread(this);
}

DeviceThread::~DeviceThread() {
	interrupt();
	wait();
}

Enum::ReceiverType DeviceThread::type() const {
	return Enum::ReceiverType::Device;
}

void DeviceThread::run() {
	Functional::ColorStream stream;
	Functional::LoopSync sync;
	const auto configs = {
		m_device->config().ribbon(0),
		m_device->config().ribbon(1),
		m_device->config().ribbon(2),
		m_device->config().ribbon(3)
	};

	do {
		if (!isEmitterConnected()) {
			sync.wait(10);
			continue;
		}

		auto source = data();

		for (const auto &config : configs) {
			double step = static_cast<double>(source.linesize()) / static_cast<double>(config.count() - 1);

			for (int i = 0; i < config.count(); ++i) {
				auto index = std::min(static_cast<int>(source.linesize() - 1), static_cast<int>(i * step));
				auto color = source.data(config.position())[index];
				stream.insert(config.colorFormat(), execute(color));
			}
		}

		stream.write(*m_device);
		m_device->waitForBytesWritten(100);
		m_device->clear();
		sync.wait(100);
	} while (!m_interrupt && m_device->error() == 0);

	if (m_device->isOpen() && m_device->isWritable())
		m_device->close();
}

QSerialPortInfo DeviceThread::details() {
	return m_details;
}

void DeviceThread::interrupt() {
	m_interrupt = true;
}

Container::DeviceConfigContainer DeviceThread::config() {
	return m_device->config();
}
