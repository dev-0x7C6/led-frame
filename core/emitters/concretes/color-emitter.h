#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QColor>

namespace Emitter {
namespace Concrete {

class ColorEmitter final : public Abstract::AbstractEmitter {
public:
	explicit ColorEmitter();

	EmitterType type() const final { return EmitterType::Color; }
	u32 framerate() const final { return 1; }

	QColor color() const;
	void setColor(const QColor &color);

private:
	QColor m_color{};
};
} // namespace Concrete
} // namespace Emitter
