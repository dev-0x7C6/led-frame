#pragma once

#include <core/enums/screen-capture-type.h>
#include <core/types.h>
#include <core/enums/color-format-enum.h>

namespace Interface {

class IScreenCapture {
public:
	explicit IScreenCapture() = default;
	virtual ~IScreenCapture() = default;

	virtual Enum::ScreenCaptureType type() const = 0;

	virtual bool capture(ci32 id = 0) = 0;

	virtual auto data() const noexcept -> ccolor * = 0;
	virtual auto width() const noexcept -> u32 = 0;
	virtual auto height() const noexcept -> u32 = 0;
	virtual auto bytesPerPixel() const noexcept -> u32 = 0;
};
} // namespace Interface
