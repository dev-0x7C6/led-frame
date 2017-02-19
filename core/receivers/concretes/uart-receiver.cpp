#include <core/containers/device-config-container.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/correctors/concretes/corrector-manager.h>
#include <core/devices/device-port.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-stream.h>
#include <core/functionals/loop-sync.h>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/emitters/interfaces/iemitter.h>
#include <core/receivers/concretes/uart-worker.h>

#include <QElapsedTimer>
#include <algorithm>
#include <memory>

using namespace Container;
using namespace Functional;
using namespace Enum;
using namespace Receiver::Abstract;
using namespace Receiver::Concrete;

UartReceiver::UartReceiver(ci32 id, std::unique_ptr<DevicePort> &&device)
		: AbstractReceiver(id)
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

#include <iostream>

Enum::ReceiverType UartReceiver::type() const { return Enum::ReceiverType::Uart; }

void UartReceiver::run() {
	const std::array<RibbonConfiguration, 4> ribbon{{
		m_device->config().ribbon(0),
		m_device->config().ribbon(1),
		m_device->config().ribbon(2),
		m_device->config().ribbon(3),
	}};

	UartWorker worker(ribbon, correctorManager(), m_device);
	Functional::LoopSync loopSync;

	Container::Scanline prev(0u);
	Container::Scanline diff(0u);
	Container::Scanline next(0u);
	Container::Scanline output(0u);
	const auto uartFramerate = framerate();
	u32 frameCounter = 0;
	int lastEmitterId = -1;

	while (!m_interrupt && m_device->error() == 0 && m_device->isDataTerminalReady()) {
		if (!isEmitterConnected() || !connectedEmitter()->isFirstFrameReady()) {
			loopSync.wait(10);
			continue;
		}

		const auto emitter = connectedEmitter();
		const auto emitterId = emitter->id();
		const auto emitterFramerate = emitter->framerate();
		const auto emitterGetFrame = [&emitter] { return emitter->data(); };

		const auto resetFrame = [&emitterGetFrame, &output, &prev, &diff, &next] {
			output = emitterGetFrame();
			prev = output;
			diff = output;
			next = output;
		};

		if (lastEmitterId == -1) {
			lastEmitterId = emitterId;
			worker.fade(emitterGetFrame);
			resetFrame();
			continue;
		}

		if (lastEmitterId != emitterId) {
			lastEmitterId = emitterId;
			worker.change(output, emitterGetFrame);
			resetFrame();
			continue;
		}

		if (emitterFramerate != 0) {
			next = emitterGetFrame();
			frameCounter++;

			if (next != diff) {
				prev = diff;
				diff = next;
				frameCounter = 0;
			}

			auto factor = std::min(1.0f, static_cast<cfactor>(frameCounter) / (static_cast<cfactor>(uartFramerate) / static_cast<cfactor>(emitterFramerate)));

			if (factor < 1.0f) {
				output = emitterGetFrame();
			} else {
				Container::Scanline::interpolate(prev, next, factor, output);
			}
		} else {
			output = emitterGetFrame();
		}

		worker.write(output);
		loopSync.wait(uartFramerate);
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
