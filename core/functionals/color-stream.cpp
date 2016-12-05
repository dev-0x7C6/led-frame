#include "color-stream.h"
#include "color-functions.h"

#include <QIODevice>

using namespace Enum;
using namespace Functional;
using namespace Functional::Color;

void ColorStream::insert(Enum::ColorFormat format, color value) {
	switch (format) {
		case ColorFormat::RGB: return writeRGB(value);
		case ColorFormat::RBG: return writeRBG(value);
		case ColorFormat::GRB: return writeGRB(value);
		case ColorFormat::BRG: return writeBRG(value);
		case ColorFormat::GBR: return writeGBR(value);
		case ColorFormat::BGR: return writeBGR(value);
	}
}

void ColorStream::write(QIODevice &device) {
	device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<int>(m_buffer.size()));
	m_seek = 0;
}

void ColorStream::writeRGB(color value) noexcept {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(value));
	m_seek = seek;
}

void ColorStream::writeRBG(color value) noexcept {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(value));
	m_seek = seek;
}

void ColorStream::writeGRB(color value) noexcept {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(value));
	m_seek = seek;
}

void ColorStream::writeGBR(color value) noexcept {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(value));
	m_seek = seek;
}

void ColorStream::writeBRG(color value) noexcept {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(value));
	m_seek = seek;
}

void ColorStream::writeBGR(color value) noexcept {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(value));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(value));
	m_seek = seek;
}
