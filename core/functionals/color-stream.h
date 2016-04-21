#pragma once

#include <core/containers/color-correction-container.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/color-type-enum.h>

#include <array>
#include <cstdint>

class QIODevice;

namespace Functional {

	class ColorStream final {
	public:
		explicit ColorStream();
		virtual ~ColorStream() = default;

		void insert(const Enum::ColorFormat &format, const uint32_t &color);
		void write(QIODevice &device);

	protected:
		inline uint32_t getr(const uint32_t &color) const;
		inline uint32_t getg(const uint32_t &color) const;
		inline uint32_t getb(const uint32_t &color) const;

		inline void writeRGB(const uint32_t &r, const uint32_t &g, const uint32_t &b);
		inline void writeRBG(const uint32_t &r, const uint32_t &g, const uint32_t &b);
		inline void writeGRB(const uint32_t &r, const uint32_t &g, const uint32_t &b);
		inline void writeGBR(const uint32_t &r, const uint32_t &g, const uint32_t &b);
		inline void writeBRG(const uint32_t &r, const uint32_t &g, const uint32_t &b);
		inline void writeBGR(const uint32_t &r, const uint32_t &g, const uint32_t &b);

	private:
		std::array<uint32_t, 270> m_buffer;
		size_t m_seek;

	};

}
