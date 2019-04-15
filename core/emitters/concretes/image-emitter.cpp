#include <core/emitters/concretes/image-emitter.h>
#include <core/enums/position-enum.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>

#include <algorithm>

#include <QFile>
#include <QDir>

#include <QCamera>
#include <QCameraInfo>
#include <QAbstractVideoSurface>

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;
using namespace Container;
using namespace Functional;

ImageEmitter::ImageEmitter(const i32 id)
		: AbstractEmitter(id) {
	loadFromFile(QDir::homePath() + QDir::separator() + "test.jpg");
}

ImageEmitter::ImageEmitter(const i32 id, const QString &filePath)
		: AbstractEmitter(id) {
	loadFromFile(filePath);
}

bool ImageEmitter::loadFromFile(const QString &filePath) {
	if (!QFile::exists(filePath))
		return false;

	m_filePath = filePath;
	auto image = QImage(filePath).convertToFormat(QImage::Format_ARGB32);

	ImageBlockProcessor<ColorAveragingBuffer, 9, 16> processor;

	processor.process(reinterpret_cast<const color *>(image.constBits()), image.width(), image.height());
	commit(processor.output());
	return true;
}

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

CameraEmitter::CameraEmitter(i32 id)
		: Abstract::AbstractEmitter(id) {

	auto list = QCameraInfo::availableCameras();

	m_captureHandle = std::make_unique<QCamera>(list.first());
	m_captureVideo = std::make_unique<CaptureRGB32>([this](auto &&scanline) { commit(scanline); });
	m_captureHandle->setViewfinder(m_captureVideo.get());
	m_captureHandle->start();
}

CameraEmitter::~CameraEmitter() = default;
