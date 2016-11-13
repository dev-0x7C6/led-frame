#include <core/containers/color-scanline-container.h>
#include "core/functionals/color-functions.h"

#include <algorithm>

using namespace Container;
using namespace Functional::Color;

ColorScanlineContainer::ColorScanlineContainer(const uint32_t fillColor) {
	fill(fillColor);
}

Enum::ContainerType ColorScanlineContainer::type() const {
	return Enum::ContainerType::ColorScanline;
}

Enum::Position ColorScanlineContainer::fromIndexToPosition(const uint32_t &index) {
	return static_cast<Enum::Position>(index / scanline_line);
}

void ColorScanlineContainer::clear() {
	m_data.fill(0u);
}

void ColorScanlineContainer::fill(const uint32_t &color) {
	m_data.fill(color);
}

void ColorScanlineContainer::rotate(const uint32_t &color) {
	std::rotate(m_data.begin(), m_data.begin() + 1, m_data.end());
	m_data[scanline_size - 1] = color;
}

void ColorScanlineContainer::interpolate(const ColorScanlineContainer &start, const ColorScanlineContainer &end, double p, ColorScanlineContainer &out) {
	for (size_t i = 0; i < scanline_size - 1; ++i) {
		const auto start_color = start.constData()[i];
		const auto end_color = end.constData()[i];

		auto r = static_cast<uint32_t>(getR(end_color) * p + (getR(start_color) * (1.0 - p)));
		auto g = static_cast<uint32_t>(getG(end_color) * p + (getG(start_color) * (1.0 - p)));
		auto b = static_cast<uint32_t>(getB(end_color) * p + (getB(start_color) * (1.0 - p)));

		out.data()[i] = rgb(r, g, b);
	}
}

void ColorScanlineContainer::fill(const Enum::Position &position, const uint32_t &color) {
	auto colors = data(position);

	for (uint32_t i = 0; i < scanline_line; ++i)
		colors[i] = color;
}
