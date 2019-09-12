#pragma once

#include <algorithm>
#include <core/enums/color-format-enum.h>
#include <core/enums/color-type-enum.h>
#include <core/types.h>
#include "color-functions.h"

#include <QIODevice>
#include <array>

class QIODevice;

namespace Functional {

template <size_t buffer_size, typename color_type = u8>
class ColorStream final {
public:
	void insert(Enum::ColorFormat format, color value) {
		switch (format) {
			case Enum::ColorFormat::RGB: return write(value, rgb_format_overload{});
			case Enum::ColorFormat::RBG: return write(value, rbg_format_overload{});
			case Enum::ColorFormat::GRB: return write(value, grb_format_overload{});
			case Enum::ColorFormat::BRG: return write(value, brg_format_overload{});
			case Enum::ColorFormat::GBR: return write(value, gbr_format_overload{});
			case Enum::ColorFormat::BGR: return write(value, bgr_format_overload{});
		}
	}

	void write(QIODevice &device) {
		device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<int>(m_buffer.size()));
		m_seek = 0;
	}

	void write(color value, rgb_format_overload) noexcept {
		m_buffer[m_seek++] = static_cast<color_type>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_b24(value));
	}

	void write(color value, rbg_format_overload) noexcept {
		m_buffer[m_seek++] = static_cast<color_type>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_g24(value));
	}

	void write(color value, grb_format_overload) noexcept {
		m_buffer[m_seek++] = static_cast<color_type>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_b24(value));
	}

	void write(color value, gbr_format_overload) noexcept {
		m_buffer[m_seek++] = static_cast<color_type>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_r24(value));
	}

	void write(color value, brg_format_overload) noexcept {
		m_buffer[m_seek++] = static_cast<color_type>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_g24(value));
	}

	void write(color value, bgr_format_overload) noexcept {
		m_buffer[m_seek++] = static_cast<color_type>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type>(get_r24(value));
	}

	constexpr static auto size() noexcept {
		return buffer_size;
	}

	auto filled() const noexcept { return m_seek; }

	const auto &buffer() const noexcept {
		m_seek = 0;
		return m_buffer;
	}

	void apply_ws2812_gamma_correction() {
		constexpr static std::array<uint8_t, 256> gamma_table{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
			2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
			5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
			10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
			17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
			25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
			37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
			51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
			69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
			90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
			115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
			144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
			177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
			215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255}};

		for (auto &color : m_buffer) {
			color = gamma_table[color];
		}
	}

private:
	std::array<color_type, buffer_size> m_buffer;
	mutable size_t m_seek{};
};

} // namespace Functional
