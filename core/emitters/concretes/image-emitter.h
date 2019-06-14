#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QImage>

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

} // namespace Concrete
} // namespace Emitter
