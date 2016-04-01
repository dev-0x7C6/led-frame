#include <core/containers/color-scanline-container.h>

using namespace Container;


Enum::ContainerType ColorScanlineContainer::type() const {
	return Enum::ContainerType::ColorScanline;
}

uint32_t *Container::ColorScanlineContainer::data(const Enum::Position &position) {
	return m_data.data() + (static_cast<int>(position) * 64);
}

uint32_t *ColorScanlineContainer::data() {
	return m_data.data();
}

Enum::Position ColorScanlineContainer::fromIndexToPosition(const uint32_t &index) {
	return static_cast<Enum::Position>(index / linesize());
}

void ColorScanlineContainer::fill(const uint32_t &color) {
	m_data.fill(color);
}

void ColorScanlineContainer::rotate(const uint32_t &color) {
	for (int i = 0; i < m_data.size() - 1; ++i)
		m_data[i] = m_data[i + 1];

	m_data[255] = color;
}

uint32_t Container::ColorScanlineContainer::linesize() {
	return scanline_size / static_cast<int>(Enum::Position::Last);
}

uint32_t ColorScanlineContainer::resolution() {
	return scanline_size;
}
