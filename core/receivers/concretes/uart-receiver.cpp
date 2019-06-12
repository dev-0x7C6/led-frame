#include <core/containers/device-config-container.h>
#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <core/devices/device-port.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-stream.h>
#include <core/functionals/frame-pace-sync.h>
#include <core/receivers/concretes/uart-receiver.h>
#include <core/interfaces/iemitter.h>
#include <core/receivers/concretes/uart-worker.h>

#include <QElapsedTimer>

#include <algorithm>
#include <iostream>
#include <memory>

using namespace Container;
using namespace Functional;
using namespace Enum;
using namespace Receiver::Abstract;
using namespace Receiver::Concrete;

UartReceiver::UartReceiver(std::unique_ptr<DevicePort> &&device, unregister_callback &&callback)
		: m_device(std::move(device))
		, m_thread([this, unregister_callback{std::move(callback)}](const auto &interrupted) {
			run(interrupted);
			unregister_callback(*this);
		}) {
}

UartReceiver::~UartReceiver() = default;

auto UartReceiver::type() const noexcept -> ReceiverType {
	return ReceiverType::Uart;
}

void UartReceiver::run(const std::atomic_bool &interrupted) {
	const std::array<RibbonConfiguration, 4> ribbon{{
		m_device->config().ribbon(0),
		m_device->config().ribbon(1),
		m_device->config().ribbon(2),
		m_device->config().ribbon(3),
	}};

	UartWorker worker(ribbon, correctors(), m_device);
	Functional::FramePaceSync framePaceing(90);
	std::optional<int> lastEmitterId;

	Scanline frame;

	while (!interrupted && worker.isValid()) {
		if (!isEmitterConnected() || !connectedEmitter()->isFirstFrameReady()) {
			framePaceing.synchronize();
			continue;
		}

		const auto emitter = connectedEmitter();
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
			worker.change(frame, emitterGetFrame, framePaceing);
			frame = emitterGetFrame();
			continue;
		}

		frame = emitterGetFrame();
		worker.write(frame, framePaceing);
	};

	worker.fadeOut([&frame]() { return frame; }, framePaceing);
}

Container::DeviceConfigContainer UartReceiver::config() { return m_device->config(); }
