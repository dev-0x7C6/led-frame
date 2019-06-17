#include "camera-emitter.h"

#include <core/emitters/concretes/camera-emitter.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>

#include <QAbstractVideoSurface>
#include <QCamera>
#include <QCameraInfo>
#include <QEventLoop>

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;
using namespace Container;
using namespace Functional;

class CaptureRGB32 : public QAbstractVideoSurface {
public:
	CaptureRGB32(std::function<void(const Scanline &)> &&update)
			: m_update(std::move(update)) {}

private:
	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType) const final {
		return {QVideoFrame::Format_ARGB32};
	}

	bool present(const QVideoFrame &buffer) final {
		QVideoFrame frame(buffer);
		frame.map(QAbstractVideoBuffer::MapMode::ReadOnly);
		ImageBlockProcessor<ColorAveragingBuffer, 9, 16> processor;
		processor.process(reinterpret_cast<const color *>(frame.bits()), frame.width(), frame.height());
		m_update(processor.output());
		return true;
	}

	std::function<void(const Scanline &)> m_update;
};

CameraEmitter::CameraEmitter() {
	auto list = QCameraInfo::availableCameras();

	if (list.isEmpty())
		return;

	m_info = list.first();
	start();
}

CameraEmitter::~CameraEmitter() {
	m_interrupted = true;
	wait();
}

void CameraEmitter::run() {
	QEventLoop loop;
	std::unique_ptr<QCamera> handle{nullptr};
	auto captureFrame = std::make_unique<CaptureRGB32>([this](auto &&scanline) { commit(std::forward<decltype(scanline)>(scanline)); });

	while (!m_interrupted) {
		while (usages() == 0) {
			if (m_interrupted)
				return;

			if (handle)
				handle.reset(nullptr);

			loop.processEvents(QEventLoop::AllEvents, 10);
		}

		if (!handle) {
			handle = std::make_unique<QCamera>(m_info);
			handle->setViewfinder(captureFrame.get());
			handle->start();
		}

		loop.processEvents(QEventLoop::ProcessEventsFlag::AllEvents);
	}
}
