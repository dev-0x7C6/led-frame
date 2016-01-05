#include <core/containers/color-scanline-container.h>

using namespace Container;


Enum::ContainerType ColorScanlineContainer::type() const {
	return Enum::ContainerType::ColorScanline;
}

std::array<unsigned int, 64> &Container::ColorScanlineContainer::data(const Enum::Position &position) {
	return m_data[static_cast<size_t>(position)];
}

void ColorScanlineContainer::fill(unsigned int color) {
	for (auto &array : m_data)
		array.fill(color);
}
