#pragma once

#include <core/enums/screen-capture-type.h>
#include <cstdint>

namespace Interface {

class IScreenCapture {
public:
	explicit IScreenCapture() = default;
	virtual ~IScreenCapture() = default;

	virtual Enum::ScreenCaptureType type() const = 0;

	virtual bool capture(const int32_t x, const int32_t y, const uint32_t w, const uint32_t h) = 0;

	virtual const uint32_t *data() = 0;
};
}
