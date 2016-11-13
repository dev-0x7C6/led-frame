#include "color-stream.h"
#include "color-functions.h"

#include <QIODevice>

using namespace Enum;
using namespace Functional;
using namespace Functional::Color;

ColorStream::ColorStream()
		: m_seek(0) {}

void ColorStream::insert(Enum::ColorFormat format, uint32_t color) {
	switch (format) {
		case ColorFormat::RGB: return writeRGB(color);
		case ColorFormat::RBG: return writeRBG(color);
		case ColorFormat::GRB: return writeGRB(color);
		case ColorFormat::BRG: return writeBRG(color);
		case ColorFormat::GBR: return writeGBR(color);
		case ColorFormat::BGR: return writeBGR(color);
	}
}

void ColorStream::write(QIODevice &device) {
	device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<int>(m_buffer.size()));
	m_seek = 0;
}

void ColorStream::writeRGB(uint32_t color) {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(color));
	m_seek = seek;
}

void ColorStream::writeRBG(uint32_t color) {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(color));
	m_seek = seek;
}

void ColorStream::writeGRB(uint32_t color) {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(color));
	m_seek = seek;
}

void ColorStream::writeGBR(uint32_t color) {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(color));
	m_seek = seek;
}

void ColorStream::writeBRG(uint32_t color) {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(color));
	m_seek = seek;
}

void ColorStream::writeBGR(uint32_t color) {
	auto seek = m_seek;
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getB(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getG(color));
	m_buffer[seek++] = static_cast<decltype(m_buffer)::value_type>(getR(color));
	m_seek = seek;
}
