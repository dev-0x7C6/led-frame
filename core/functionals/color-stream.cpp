#include "color-stream.h"
#include "color-functions.h"

#include <QIODevice>

using namespace Enum;
using namespace Functional;
using namespace Functional::Color;

void ColorStream::insert(Enum::ColorFormat format, color value) {
	switch (format) {
		case ColorFormat::RGB: return write(value, rgb_format{});
		case ColorFormat::RBG: return write(value, rbg_format{});
		case ColorFormat::GRB: return write(value, grb_format{});
		case ColorFormat::BRG: return write(value, brg_format{});
		case ColorFormat::GBR: return write(value, gbr_format{});
		case ColorFormat::BGR: return write(value, bgr_format{});
	}
}

void ColorStream::write(QIODevice &device) {
	device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<int>(m_buffer.size()));
	m_seek = 0;
}

void ColorStream::write(color value, rgb_format) noexcept {
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
}

void ColorStream::write(color value, rbg_format) noexcept {
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
}

void ColorStream::write(color value, grb_format) noexcept {
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
}

void ColorStream::write(color value, gbr_format) noexcept {
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
}

void ColorStream::write(color value, brg_format) noexcept {
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
}

void ColorStream::write(color value, bgr_format) noexcept {
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_b24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_g24(value));
	m_buffer[m_seek++] = static_cast<color_type_u8>(get_r24(value));
}
