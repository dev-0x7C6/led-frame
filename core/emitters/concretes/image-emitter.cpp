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
	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType) const final {
		return {QVideoFrame::Format_RGB32};
	}

	bool present(const QVideoFrame &) final {
		return true;
	}
};

CameraEmitter::CameraEmitter(i32 id)
		: Abstract::AbstractEmitter(id) {

	auto list = QCameraInfo::availableCameras();

	m_captureHandle = std::make_unique<QCamera>(list.first());
	m_captureHandle->start();
}

CameraEmitter::~CameraEmitter() = default;
