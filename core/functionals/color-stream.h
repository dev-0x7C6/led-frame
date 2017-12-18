#pragma once

#include <core/enums/color-format-enum.h>
#include <core/enums/color-type-enum.h>
#include <core/types.h>

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
	void write(color value, rgb_format) noexcept;
	void write(color value, rbg_format) noexcept;
	void write(color value, grb_format) noexcept;
	void write(color value, gbr_format) noexcept;
	void write(color value, brg_format) noexcept;
	void write(color value, bgr_format) noexcept;

private:
	using color_type_u8 = u8;
	std::array<color_type_u8, 270> m_buffer;
	size_t m_seek = 0;
};
}
