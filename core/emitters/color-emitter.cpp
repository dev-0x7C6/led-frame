#include <core/emitters/color-emitter.h>

using namespace Emitter;

ColorEmitter::ColorEmitter() {
	setColor(Qt::darkBlue);
}

QString ColorEmitter::name() const {
	return "Color";
}

Enum::EmitterType ColorEmitter::type() const {
	return Enum::EmitterType::Color;
}

QColor ColorEmitter::color() const {
	return m_color;
}

void ColorEmitter::setColor(const QColor &color) {
	m_color = color;
	m_scanline.fill(color.rgb());
	commit(m_scanline);
}
