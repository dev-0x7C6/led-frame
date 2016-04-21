#include <core/functionals/color-stream.h>
#include <QIODevice>

using namespace Enum;
using namespace Functional;

ColorStream::ColorStream()
	: m_seek(0)

{
}

void ColorStream::insert(const Enum::ColorFormat &format, const uint32_t &color) {
	const auto r = getr(color);
	const auto g = getg(color);
	const auto b = getb(color);

	switch (format) {
		case ColorFormat::RGB:
			return writeRGB(r, g, b);

		case ColorFormat::RBG:
			return writeRBG(r, g, b);

		case ColorFormat::GRB:
			return writeGRB(r, g, b);

		case ColorFormat::BRG:
			return writeBRG(r, g, b);

		case ColorFormat::GBR:
			return writeGBR(r, g, b);

		case ColorFormat::BGR:
			return writeBGR(r, g, b);
	}
}

void ColorStream::write(QIODevice &device) {
	device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<int>(m_buffer.size()));
	m_seek = 0;
}

uint32_t ColorStream::getr(const uint32_t &color) const {
	return (color >> 16) & 0xff;
}

uint32_t ColorStream::getg(const uint32_t &color) const {
	return (color >> 8) & 0xff;
}

uint32_t ColorStream::getb(const uint32_t &color) const {
	return color & 0xff;
}

void ColorStream::writeRGB(const uint32_t &r, const uint32_t &g, const uint32_t &b) {
	m_buffer[m_seek + 0] = r;
	m_buffer[m_seek + 1] = g;
	m_buffer[m_seek + 2] = b;
	m_seek += 3;
}

void ColorStream::writeRBG(const uint32_t &r, const uint32_t &g, const uint32_t &b) {
	m_buffer[m_seek + 0] = r;
	m_buffer[m_seek + 1] = b;
	m_buffer[m_seek + 2] = g;
	m_seek += 3;
}

void ColorStream::writeGRB(const uint32_t &r, const uint32_t &g, const uint32_t &b) {
	m_buffer[m_seek + 0] = g;
	m_buffer[m_seek + 1] = r;
	m_buffer[m_seek + 2] = b;
	m_seek += 3;
}

void ColorStream::writeGBR(const uint32_t &r, const uint32_t &g, const uint32_t &b) {
	m_buffer[m_seek + 0] = g;
	m_buffer[m_seek + 1] = b;
	m_buffer[m_seek + 2] = r;
	m_seek += 3;
}

void ColorStream::writeBRG(const uint32_t &r, const uint32_t &g, const uint32_t &b) {
	m_buffer[m_seek + 0] = b;
	m_buffer[m_seek + 1] = r;
	m_buffer[m_seek + 2] = g;
	m_seek += 3;
}

void ColorStream::writeBGR(const uint32_t &r, const uint32_t &g, const uint32_t &b) {
	m_buffer[m_seek + 0] = b;
	m_buffer[m_seek + 1] = g;
	m_buffer[m_seek + 2] = r;
	m_seek += 3;
}
