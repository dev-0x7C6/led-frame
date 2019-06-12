#include <core/emitters/concretes/color-emitter.h>

using namespace Emitter::Concrete;

ColorEmitter::ColorEmitter() {
	setColor(Qt::darkGreen);
}

QColor ColorEmitter::color() const {
	return m_color;
}

void ColorEmitter::setColor(const QColor &color) {
	if (color == m_color)
		return;

	m_color = color;
	commit(Container::Scanline(color.rgb()));
}
