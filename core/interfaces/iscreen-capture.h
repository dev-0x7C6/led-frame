#pragma once

#include <core/enums/screen-capture-type.h>
#include <cstdint>

namespace Interface {

class IScreenCapture {
public:
	explicit IScreenCapture() = default;
	virtual ~IScreenCapture() = default;

	virtual Enum::ScreenCaptureType type() const = 0;

	virtual int32_t width() = 0;
	virtual int32_t height() = 0;

	virtual bool capture() = 0;
	virtual const uint32_t *data() = 0;
};
}
