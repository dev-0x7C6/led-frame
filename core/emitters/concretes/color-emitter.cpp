#include <core/emitters/concretes/color-emitter.h>

using namespace Emitter::Concrete;

ColorEmitter::ColorEmitter(ci32 id)
		: AbstractEmitter(id) {
	setColor(Qt::darkGreen);
}

QColor ColorEmitter::color() const {
	return m_color;
}

void ColorEmitter::setColor(const QColor &color) {
	if (color == m_color)
		return;

	m_color = color;
	m_scanline.fill(color.rgb());
	commit(m_scanline);
}
