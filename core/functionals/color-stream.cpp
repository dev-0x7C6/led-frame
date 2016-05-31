#include <core/functionals/color-stream.h>
#include <QIODevice>

using namespace Enum;
using namespace Functional;

ColorStream::ColorStream()
		: m_seek(0) {}

void ColorStream::insert(Enum::ColorFormat format, uint32_t color) {
	switch (format) {
		case ColorFormat::RGB:
			return writeRGB(color);

		case ColorFormat::RBG:
			return writeRBG(color);

		case ColorFormat::GRB:
			return writeGRB(color);

		case ColorFormat::BRG:
			return writeBRG(color);

		case ColorFormat::GBR:
			return writeGBR(color);

		case ColorFormat::BGR:
			return writeBGR(color);
	}
}

void ColorStream::write(QIODevice &device) {
	device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<int>(m_buffer.size()));
	m_seek = 0;
}

void ColorStream::writeRGB(uint32_t color) {
	const auto seek = m_seek;
	m_buffer[seek + 0] = color >> 0x10 & 0xffu; // r
	m_buffer[seek + 1] = color >> 0x08 & 0xffu; // g
	m_buffer[seek + 2] = color >> 0x00 & 0xffu; // b
	m_seek += 3;
}

void ColorStream::writeRBG(uint32_t color) {
	const auto seek = m_seek;
	m_buffer[seek + 0] = color >> 0x10 & 0xffu; // r
	m_buffer[seek + 1] = color >> 0x00 & 0xffu; // b
	m_buffer[seek + 2] = color >> 0x08 & 0xffu; // g
	m_seek += 3;
}

void ColorStream::writeGRB(uint32_t color) {
	const auto seek = m_seek;
	m_buffer[seek + 0] = color >> 0x08 & 0xffu; // g
	m_buffer[seek + 1] = color >> 0x10 & 0xffu; // r
	m_buffer[seek + 2] = color >> 0x00 & 0xffu; // b
	m_seek += 3;
}

void ColorStream::writeGBR(uint32_t color) {
	const auto seek = m_seek;
	m_buffer[seek + 0] = color >> 0x08 & 0xffu; // g
	m_buffer[seek + 1] = color >> 0x00 & 0xffu; // b
	m_buffer[seek + 2] = color >> 0x10 & 0xffu; // r
	m_seek += 3;
}

void ColorStream::writeBRG(uint32_t color) {
	const auto seek = m_seek;
	m_buffer[seek + 0] = color >> 0x00 & 0xffu; // b
	m_buffer[seek + 1] = color >> 0x10 & 0xffu; // r
	m_buffer[seek + 2] = color >> 0x08 & 0xffu; // g
	m_seek += 3;
}

void ColorStream::writeBGR(uint32_t color) {
	const auto seek = m_seek;
	m_buffer[seek + 0] = color >> 0x00 & 0xffu; // b
	m_buffer[seek + 1] = color >> 0x08 & 0xffu; // g
	m_buffer[seek + 2] = color >> 0x10 & 0xffu; // r
	m_seek += 3;
}
