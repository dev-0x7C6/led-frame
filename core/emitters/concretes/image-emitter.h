#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QImage>

namespace Emitter {
namespace Concrete {

class ImageEmitter final : public Abstract::AbstractEmitter {
public:
	explicit ImageEmitter(ci32 id);
	explicit ImageEmitter(ci32 id, const QString &filePath);
	virtual ~ImageEmitter() = default;

	virtual EmitterType type() const override;
	virtual u32 framerate() const override { return 1; }

	QRect fragment(int w, int h, cu32 index);

	bool loadFromFile(const QString &filePath);

private:
	QImage m_image;
	QString m_filePath;
};
} // namespace Concrete
} // namespace Emitter
