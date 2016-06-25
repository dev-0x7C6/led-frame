#include <core/containers/device-config-container.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/correctors/concretes/corrector-manager.h>
#include <core/devices/device-port.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-stream.h>
#include <core/functionals/loop-sync.h>
#include <core/receivers/concretes/uart-receiver.h>

#include <QElapsedTimer>
#include <algorithm>
#include <memory>

using namespace Container;
using namespace Functional;
using namespace Enum;
using namespace Receiver::Abstract;
using namespace Receiver::Concrete;

UartReceiver::UartReceiver(std::unique_ptr<DevicePort> &&device, QSerialPortInfo details)
		: AbstractReceiver()
		, m_device(std::move(device))
		, m_details(details)
		, m_interrupt(false)
		, m_thread([this]() { run(); }, this) {
	m_device->moveToThread(&m_thread);
	m_thread.start();
	connect(&m_thread, &QThread::finished, this, &UartReceiver::finished);
}

UartReceiver::~UartReceiver() {
	interrupt();
	m_thread.wait();
}

Enum::ReceiverType UartReceiver::type() const { return Enum::ReceiverType::Uart; }

void UartReceiver::run() {
	Functional::ColorStream stream;
	Functional::LoopSync sync;
	const auto configs = {
		m_device->config().ribbon(0),
		m_device->config().ribbon(1),
		m_device->config().ribbon(2),
		m_device->config().ribbon(3)};
	Container::ColorScanlineContainer source;

	while (!m_interrupt && m_device->error() == 0 &&
		m_device->isDataTerminalReady()) {
		if (!isEmitterConnected()) {
			sync.wait(10);
			continue;
		}

		source = data();

		correctorManager()->push();

		for (const auto &config : configs) {
			double step = static_cast<double>(scanline_line) / static_cast<double>(config.count() - 1);

			for (int i = 0; i < config.count(); ++i) {
				auto index = std::min(static_cast<int>(scanline_line - 1), static_cast<int>(i * step));
				auto color = source.data(config.position())[index];
				stream.insert(config.colorFormat(), correctorManager()->execute(color));
			}
		}

		correctorManager()->pop();

		stream.write(*m_device);
		m_device->waitForBytesWritten(-1);
		sync.wait(90);
	};

	if (m_device->isOpen() && m_device->isWritable())
		m_device->close();
}

QSerialPortInfo UartReceiver::details() {
	return m_details;
}

void UartReceiver::interrupt() {
	m_interrupt = true;
}

Container::DeviceConfigContainer UartReceiver::config() {
	return m_device->config();
}
