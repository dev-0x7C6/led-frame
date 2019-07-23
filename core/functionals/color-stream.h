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

private:
	std::array<color_type, buffer_size> m_buffer;
	mutable size_t m_seek{};
};
} // namespace Functional
