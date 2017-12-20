#pragma once

#include <core/enums/color-format-enum.h>
#include <core/enums/color-type-enum.h>
#include <core/types.h>
#include "color-functions.h"

#include <QIODevice>
#include <array>

class QIODevice;

namespace Functional {

template <size_t buffer_size>
class ColorStream final {
public:
	explicit ColorStream() = default;
	virtual ~ColorStream() = default;

	void insert(Enum::ColorFormat format, color value) {
		switch (format) {
			case Enum::ColorFormat::RGB: return write(value, rgb_format{});
			case Enum::ColorFormat::RBG: return write(value, rbg_format{});
			case Enum::ColorFormat::GRB: return write(value, grb_format{});
			case Enum::ColorFormat::BRG: return write(value, brg_format{});
			case Enum::ColorFormat::GBR: return write(value, gbr_format{});
			case Enum::ColorFormat::BGR: return write(value, bgr_format{});
		}
	}

	void write(QIODevice &device) {
		device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<int>(m_buffer.size()));
		m_seek = 0;
	}

	void write(color value, rgb_format) noexcept {
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
	}

	void write(color value, rbg_format) noexcept {
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
	}

	void write(color value, grb_format) noexcept {
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
	}

	void write(color value, gbr_format) noexcept {
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
	}

	void write(color value, brg_format) noexcept {
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
	}

	void write(color value, bgr_format) noexcept {
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
		m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
	}

private:
	using color_type_u8 = u8;
	std::array<color_type_u8, buffer_size> m_buffer;
	size_t m_seek = 0;
};
} // namespace Functional
