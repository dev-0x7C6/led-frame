#include <core/containers/color-scanline-container.h>

using namespace Container;

Enum::ContainerType ColorScanlineContainer::type() const {
	return Enum::ContainerType::ColorScanline;
}

Enum::Position ColorScanlineContainer::fromIndexToPosition(const uint32_t &index) {
	return static_cast<Enum::Position>(index / scanline_line);
}

void ColorScanlineContainer::fill(const uint32_t &color) {
	m_data.fill(color);
}

void ColorScanlineContainer::rotate(const uint32_t &color) {
	for (size_t i = 0; i < m_data.size() - 1; ++i)
		m_data[i] = m_data[i + 1];

	m_data[scanline_size - 1] = color;
}

ColorScanlineContainer &ColorScanlineContainer::operator =(const ColorScanlineContainer &other) {
	m_data = other.m_data;
	return *this;
}

void ColorScanlineContainer::fill(const Enum::Position &position, const uint32_t &color) {
	auto colors = data(position);

	for (uint32_t i = 0; i < scanline_line; ++i)
		colors[i] = color;
}
