#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/devices/device-port.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-stream.h>
#include <core/functionals/frame-pace-sync.h>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/interfaces/iemitter.h>
#include <core/receivers/concretes/uart-worker.h>
#include <externals/common/logger/logger.hpp>

#include <QElapsedTimer>
#include <QEventLoop>
#include <QThread>

#include <algorithm>
#include <iostream>
#include <memory>

using namespace Container;
using namespace Functional;
using namespace Enum;
using namespace Receiver::Abstract;
using namespace Receiver::Concrete;

namespace {
constexpr auto filter = error_class::debug;
constexpr auto module = "[uart receiver]: ";
}

UartReceiver::UartReceiver(QSerialPortInfo &&info, unregister_callback &&callback)
		: m_thread([this, info{std::move(info)}, unregister_callback{std::move(callback)}](const auto &interrupted) {
			run(info, interrupted);
			unregister_callback(*this);
		}) {
}

UartReceiver::~UartReceiver() = default;

auto UartReceiver::type() const noexcept -> receiver_type {
	return receiver_type::uart;
}

void UartReceiver::run(const QSerialPortInfo &info, const std::atomic_bool &interrupted) {
	logger<filter>::debug(module, "thread started");
	QEventLoop loop;
	loop.thread()->setPriority(QThread::Priority::HighestPriority);

	Protocol::Concrete::LedFrameProtocol protocol(std::move(info));

	UartWorker worker(correctors(), protocol);
	Functional::FramePaceSync framePaceing(1000);
	std::optional<int> lastEmitterId;

	Scanline frame;

	logger<filter>::information(module, "running");

	while (!interrupted && worker.isValid()) {
		const auto emitter = connectedEmitter();

		if (!isEmitterConnected() || !emitter->isFirstFrameReady()) {
			framePaceing.synchronize();
			continue;
		}

		if (!emitter->isValid()) {
			logger<filter>::notice(module, "receiver ", name(), ": connected emitter ", emitter->name(), " no longer valid, disconnecting from receiver");
			connectEmitter(nullptr);
			continue;
		}

		const auto emitterId = emitter->id();
		auto emitterGetFrame = [&emitter] { return emitter->data(); };

		if (!lastEmitterId.has_value()) {
			lastEmitterId = emitterId;
			worker.fadeIn(emitterGetFrame, framePaceing);
			frame = emitterGetFrame();
			continue;
		}

		if (lastEmitterId != emitterId) {
			lastEmitterId = emitterId;
			worker.change(
				frame, [&emitter] { return emitter->data(); }, framePaceing);
			frame = emitterGetFrame();
			continue;
		}

		frame = emitterGetFrame();
		worker.write(frame, framePaceing);
		loop.processEvents(QEventLoop::AllEvents, 1);
	}

	logger<filter>::hint(module, "receiver fadeout");
	worker.fadeOut([&frame]() { return frame; }, framePaceing);
	logger<filter>::debug(module, "thread finished");
}
