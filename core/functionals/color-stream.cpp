#include <core/functionals/color-stream.h>
#include <QIODevice>

using namespace Enum;
using namespace Functional;

ColorStream::ColorStream()
	: m_seek(0)

{
}

void ColorStream::insert(const Enum::ColorFormat &format, unsigned int color) {
	const unsigned char r = static_cast<unsigned char>(std::min(red(color), 255.0));
	const unsigned char g = static_cast<unsigned char>(std::min(green(color), 255.0));
	const unsigned char b = static_cast<unsigned char>(std::min(blue(color), 255.0));

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
	device.write(reinterpret_cast<const char *>(m_buffer.data()), static_cast<qint64>(m_seek));
	m_seek = 0;
}

Container::ColorCorrectionContainer &ColorStream::correction() {
	return m_correction;
}

double ColorStream::red(unsigned int color) const {
	return static_cast<double>(((color >> 16) & 0xff)) *
	       m_correction.color(Enum::ColorType::Red) * m_correction.color(Enum::ColorType::Light);
}

double ColorStream::green(unsigned int color) const {
	return static_cast<double>(((color >> 8) & 0xff)) *
	       m_correction.color(Enum::ColorType::Green) * m_correction.color(Enum::ColorType::Light);
}

double ColorStream::blue(unsigned int color) const {
	return static_cast<double>((color & 0xff)) *
	       m_correction.color(Enum::ColorType::Blue) * m_correction.color(Enum::ColorType::Light);
}

void ColorStream::writeRGB(unsigned char r, unsigned char g, unsigned char b) {
	m_buffer[m_seek++] = r;
	m_buffer[m_seek++] = g;
	m_buffer[m_seek++] = b;
}

void ColorStream::writeRBG(unsigned char r, unsigned char g, unsigned char b) {
	m_buffer[m_seek++] = r;
	m_buffer[m_seek++] = b;
	m_buffer[m_seek++] = g;
}

void ColorStream::writeGRB(unsigned char r, unsigned char g, unsigned char b) {
	m_buffer[m_seek++] = g;
	m_buffer[m_seek++] = r;
	m_buffer[m_seek++] = b;
}

void ColorStream::writeGBR(unsigned char r, unsigned char g, unsigned char b) {
	m_buffer[m_seek++] = g;
	m_buffer[m_seek++] = b;
	m_buffer[m_seek++] = r;
}

void ColorStream::writeBRG(unsigned char r, unsigned char g, unsigned char b) {
	m_buffer[m_seek++] = b;
	m_buffer[m_seek++] = r;
	m_buffer[m_seek++] = g;
}

void ColorStream::writeBGR(unsigned char r, unsigned char g, unsigned char b) {
	m_buffer[m_seek++] = b;
	m_buffer[m_seek++] = g;
	m_buffer[m_seek++] = r;
}
