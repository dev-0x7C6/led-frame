#pragma once

#include <core/containers/color-correction-container.h>
#include <core/enums/color-type-enum.h>
#include <core/enums/color-format-enum.h>

#include <array>

class QIODevice;

namespace Functional {

	class ColorStream final {
	public:
		explicit ColorStream();
		virtual ~ColorStream() = default;

		void insert(const Enum::ColorFormat &format, unsigned int color);

		void write(QIODevice &device);

		Container::ColorCorrectionContainer &correction();

	protected:
		double red(unsigned int color) const;
		double green(unsigned int color) const;
		double blue(unsigned int color) const;

		void writeRGB(unsigned char r, unsigned char g, unsigned char b);
		void writeRBG(unsigned char r, unsigned char g, unsigned char b);
		void writeGRB(unsigned char r, unsigned char g, unsigned char b);
		void writeGBR(unsigned char r, unsigned char g, unsigned char b);
		void writeBRG(unsigned char r, unsigned char g, unsigned char b);
		void writeBGR(unsigned char r, unsigned char g, unsigned char b);

	private:
		Container::ColorCorrectionContainer m_correction;
		std::array<unsigned char, 4096> m_buffer;
		size_t m_seek;

	};

}
