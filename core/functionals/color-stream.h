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

	void insert(Enum::ColorFormat format, uint32_t color);
	void write(QIODevice &device);

protected:
	inline void writeRGB(uint32_t color);
	inline void writeRBG(uint32_t color);
	inline void writeGRB(uint32_t color);
	inline void writeGBR(uint32_t color);
	inline void writeBRG(uint32_t color);
	inline void writeBGR(uint32_t color);

private:
	std::array<uint8_t, 270> m_buffer;
	size_t m_seek;
};
}
