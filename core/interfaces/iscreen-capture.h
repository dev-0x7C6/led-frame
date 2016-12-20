#pragma once

#include <core/enums/screen-capture-type.h>
#include <core/types.h>

namespace Interface {

class IScreenCapture {
public:
	explicit IScreenCapture() = default;
	virtual ~IScreenCapture() = default;

	virtual Enum::ScreenCaptureType type() const = 0;

	virtual bool capture(ci32 id = 0) = 0;

	virtual ccolor *data() = 0;
	virtual u32 width() const noexcept = 0;
	virtual u32 height() const noexcept = 0;
};
}
