#pragma once

#include "core/types.h"
#include "core/enums/color-format-enum.h"
#include "core/enums/color-type-enum.h"

#include <array>

class QIODevice;

namespace Functional {

class ColorStream final {
public:
	explicit ColorStream() = default;
	virtual ~ColorStream() = default;

	void insert(Enum::ColorFormat format, color value);
	void write(QIODevice &device);

private:
	void writeRGB(color value) noexcept;
	void writeRBG(color value) noexcept;
	void writeGRB(color value) noexcept;
	void writeGBR(color value) noexcept;
	void writeBRG(color value) noexcept;
	void writeBGR(color value) noexcept;

private:
	std::array<u8, 270> m_buffer;
	size_t m_seek = 0;
};
}
