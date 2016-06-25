#include <core/emitters/concretes/color-emitter.h>

using namespace Emitter::Concrete;

ColorEmitter::ColorEmitter() {
	setColor(Qt::darkGreen);
}

Enum::EmitterType ColorEmitter::type() const {
	return Enum::EmitterType::Color;
}

QJsonObject ColorEmitter::parameters() const {
	return {
		{"name", name()},
		{"type", static_cast<int>(type())},
		{"description", description(type())},
		{"parameters", m_color.name()}};
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
