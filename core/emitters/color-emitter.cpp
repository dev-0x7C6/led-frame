#include <core/emitters/color-emitter.h>

using namespace Emitter;

ColorEmitter::ColorEmitter() {
	setColor(Qt::darkGreen);
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

void ColorEmitter::onConnect(const uint32_t &) {
}

void ColorEmitter::onDisconnect(const uint32_t &) {
}
