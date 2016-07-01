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

void ColorScanlineContainer::interpolate(const ColorScanlineContainer &start, const ColorScanlineContainer &end, double p, ColorScanlineContainer &out) {
	for (size_t i = 0; i < scanline_size - 1; ++i) {
		const uint32_t sc = start.constData()[i];
		const uint32_t ec = end.constData()[i];
		auto sr = static_cast<uint32_t>((sc >> 0x10) & 0xffu);
		auto sg = static_cast<uint32_t>((sc >> 0x08) & 0xffu);
		auto sb = static_cast<uint32_t>((sc >> 0x00) & 0xffu);
		auto er = static_cast<uint32_t>((ec >> 0x10) & 0xffu);
		auto eg = static_cast<uint32_t>((ec >> 0x08) & 0xffu);
		auto eb = static_cast<uint32_t>((ec >> 0x00) & 0xffu);

		auto out_r = std::min(0xffu, static_cast<uint32_t>(er * p + (sr * (1.0 - p))));
		auto out_g = std::min(0xffu, static_cast<uint32_t>(eg * p + (sg * (1.0 - p))));
		auto out_b = std::min(0xffu, static_cast<uint32_t>(eb * p + (sb * (1.0 - p))));

		uint32_t color = 0;
		color |= out_r << 0x10;
		color |= out_g << 0x08;
		color |= out_b << 0x00;
		out.data()[i] = color;
	}
}

void ColorScanlineContainer::fill(const Enum::Position &position, const uint32_t &color) {
	auto colors = data(position);

	for (uint32_t i = 0; i < scanline_line; ++i)
		colors[i] = color;
}
