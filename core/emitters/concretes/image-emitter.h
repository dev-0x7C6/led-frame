#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QImage>

class QCamera;
class CaptureRGB32;

namespace Emitter {
namespace Concrete {

class ImageEmitter final : public Abstract::AbstractEmitter {
public:
	explicit ImageEmitter();
	explicit ImageEmitter(const QString &filePath);

	EmitterType type() const final { return EmitterType::Image; }
	u32 framerate() const final { return 1; }

	bool loadFromFile(const QString &filePath);

private:
	QImage m_image;
	QString m_filePath;
};

class CameraEmitter final : public Abstract::AbstractEmitter {
public:
	explicit CameraEmitter();
	~CameraEmitter();

	EmitterType type() const final { return EmitterType::Camera; }

private:
	std::unique_ptr<QCamera> m_captureHandle;
	std::unique_ptr<CaptureRGB32> m_captureVideo;
};

} // namespace Concrete
} // namespace Emitter
