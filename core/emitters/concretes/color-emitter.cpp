#include <core/emitters/concretes/color-emitter.h>

using namespace Emitter::Concrete;

ColorEmitter::ColorEmitter(int id)
		: AbstractEmitter(id) {
	setColor(Qt::darkGreen);
}

Enum::EmitterType ColorEmitter::type() const {
	return Enum::EmitterType::Color;
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
	notify();
}

void ColorEmitter::onConnect(const uint32_t &) {
}

void ColorEmitter::onDisconnect(const uint32_t &) {
}
