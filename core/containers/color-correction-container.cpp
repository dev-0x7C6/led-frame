#include <core/containers/color-correction-container.h>

using namespace Container;

ColorCorrectionContainer::ColorCorrectionContainer() {
	for (auto &value : m_color)
		value = 1.0;
}

Enum::ContainerType ColorCorrectionContainer::type() const {
	return Enum::ContainerType::ColorCorrector;
}

void ColorCorrectionContainer::setColor(const double &value, const Enum::ColorType &type) {
	m_color[static_cast<size_t>(type)] = value;
}

double ColorCorrectionContainer::color(const Enum::ColorType &type) const {
	return m_color.at(static_cast<size_t>(type));
}
