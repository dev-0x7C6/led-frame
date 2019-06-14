#include "camera-emitter.h"

#include <core/emitters/concretes/camera-emitter.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>

#include <QCamera>
#include <QCameraInfo>
#include <QAbstractVideoSurface>

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

	m_captureHandle = std::make_unique<QCamera>(list.first());
	m_captureVideo = std::make_unique<CaptureRGB32>([this](auto &&scanline) { commit(std::forward<decltype(scanline)>(scanline)); });
	m_captureHandle->setViewfinder(m_captureVideo.get());
	m_captureHandle->start();
}

CameraEmitter::~CameraEmitter() = default;
