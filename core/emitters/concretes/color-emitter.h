#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QColor>

namespace Emitter {
namespace Concrete {

class ColorEmitter final : public Abstract::AbstractEmitter {
public:
	explicit ColorEmitter(ci32 id);
	virtual ~ColorEmitter() = default;

	virtual Enum::EmitterType type() const override;
	virtual u32 framerate() const override { return 1; }

	QColor color() const;
	void setColor(const QColor &color);

private:
	Container::Scanline m_scanline;
	QColor m_color;
};
}
}
