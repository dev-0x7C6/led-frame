#include "camera-emitter.h"

#include <core/emitters/concretes/camera-emitter.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>
#include <externals/common/logger/logger.hpp>

#include <QAbstractVideoSurface>
#include <QCamera>
#include <QCameraInfo>
#include <QEventLoop>

#include <thread>
#include <chrono>

namespace {
constexpr auto module = "[camera_capture]: ";
constexpr auto filter = error_class::debug;
} // namespace

using namespace std::chrono_literals;

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;
using namespace Container;
using namespace Functional;

template <QVideoFrame::PixelFormat format>
class Capture : public QAbstractVideoSurface {
public:
	Capture(std::function<void(const Scanline &)> &&update)
			: m_update(std::move(update)) {}

private:
	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType) const final {
		return {format};
	}

	bool present(const QVideoFrame &buffer) final {
		QVideoFrame frame(buffer);

		if (!buffer.isValid())
			return false;

		frame.map(QAbstractVideoBuffer::MapMode::ReadOnly);
		m_update(ImageBlockProcessor<ColorAveragingBuffer, 9, 16>::process(reinterpret_cast<const color *>(frame.bits()), frame.width(), frame.height()));
		return true;
	}

	std::function<void(const Scanline &)> m_update;
};

CameraEmitter::CameraEmitter(std::any &&argument)
		: m_info(std::any_cast<QCameraInfo>(argument)) {
	start();
}

CameraEmitter::~CameraEmitter() {
	m_interrupted = true;
	wait();
}

void CameraEmitter::run() {
	QEventLoop loop;
	std::unique_ptr<QCamera> handle;
	Capture<QVideoFrame::Format_ARGB32> capture([this](auto &&scanline) { commit(std::forward<decltype(scanline)>(scanline)); });

	const auto process = [&loop]() noexcept {
		loop.processEvents(QEventLoop::AllEvents, 10);
		std::this_thread::sleep_for(1ms); // avoid busy loop
	};

	while (!m_interrupted) {
		while (usages() == 0) {
			if (m_interrupted)
				return;

			if (handle)
				handle.reset(nullptr);

			process();
		}

		if (!handle) {
			handle = std::make_unique<QCamera>(m_info);
			QObject::connect(handle.get(), &QCamera::stateChanged, [this](auto &&state) {
				if (QCamera::UnloadedState == state) {
					m_interrupted = true;
				}
			});

			QCameraViewfinderSettings settings;
			settings.setResolution(1280, 720);
			settings.setMinimumFrameRate(30.0);
			settings.setMaximumFrameRate(60.0);
			settings.setPixelFormat(QVideoFrame::PixelFormat::Format_ARGB32);

			handle->setViewfinderSettings(settings);
			handle->setViewfinder(&capture);
			handle->start();
		}

		process();
	}

	logger<filter>::debug(module, "quiting");

	handle.reset(nullptr);
	process();
	invalidate();
}
