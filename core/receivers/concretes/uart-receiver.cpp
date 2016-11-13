#include <core/containers/device-config-container.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/correctors/concretes/corrector-manager.h>
#include <core/devices/device-port.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-stream.h>
#include <core/functionals/loop-sync.h>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/emitters/interfaces/iemitter.h>
#include "core/receivers/concretes/uart-worker.h"

#include <QElapsedTimer>
#include <algorithm>
#include <memory>

using namespace Container;
using namespace Functional;
using namespace Enum;
using namespace Receiver::Abstract;
using namespace Receiver::Concrete;

UartReceiver::UartReceiver(std::unique_ptr<DevicePort> &&device)
		: AbstractReceiver()
		, m_device(std::move(device))
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
	const std::vector<LedRibbonConfigContainer> ribbon = {
		m_device->config().ribbon(0),
		m_device->config().ribbon(1),
		m_device->config().ribbon(2),
		m_device->config().ribbon(3),
	};

	UartWorker worker(ribbon, correctorManager(), m_device);
	Functional::LoopSync loopSync;

	Container::ColorScanlineContainer prev(0u);
	Container::ColorScanlineContainer diff(0u);
	Container::ColorScanlineContainer next(0u);
	Container::ColorScanlineContainer output(0u);
	uint32_t frameCounter = 0;
	bool firstFrame = true;

	while (!m_interrupt && m_device->error() == 0 && m_device->isDataTerminalReady()) {
		if (!isEmitterConnected() || !connectedEmitter()->isFirstFrameReady()) {
			loopSync.wait(10);
			continue;
		}

		const auto emitter = connectedEmitter();
		const auto emitterFramerate = emitter->framerate();

		if (firstFrame) {
			firstFrame = false;
			output = emitter->data();
			worker.fade([&emitter]() { return emitter->data(); });
			output = emitter->data();
			prev = output;
			diff = output;
			next = output;
			continue;
		}

		if (emitterFramerate != 0) {
			next = emitter->data();
			frameCounter++;

			if (next != diff) {
				prev = diff;
				diff = next;
				frameCounter = 0;
			}

			auto fadeFactor = static_cast<double>(framerate()) / static_cast<double>(emitterFramerate);
			Container::ColorScanlineContainer::interpolate(prev, next, std::min(1.0, double(frameCounter) / fadeFactor), output);
		} else {
			output = emitter->data();
		}

		worker.write(output);
		loopSync.wait(m_uartFramerate);
	};

	if (m_device->isOpen() && m_device->isWritable()) {
		const auto emitter = connectedEmitter();

		if (emitter)
			worker.fade([&emitter]() { return emitter->data(); }, false);

		m_device->close();
	}
}

void UartReceiver::interrupt() { m_interrupt = true; }

Container::DeviceConfigContainer UartReceiver::config() { return m_device->config(); }
