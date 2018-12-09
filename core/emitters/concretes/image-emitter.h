#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QImage>

namespace Emitter {
namespace Concrete {

class ImageEmitter final : public Abstract::AbstractEmitter {
public:
	explicit ImageEmitter(i32 id);
	explicit ImageEmitter(i32 id, const QString &filePath);

	EmitterType type() const final { return EmitterType::Image; }
	u32 framerate() const final { return 1; }

	QRect fragment(int w, int h, u32 index);

	bool loadFromFile(const QString &filePath);

private:
	QImage m_image;
	QString m_filePath;
};
} // namespace Concrete
} // namespace Emitter
