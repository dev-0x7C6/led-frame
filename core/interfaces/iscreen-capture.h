#pragma once

#include <core/enums/screen-capture-type.h>
#include "core/types.h"

namespace Interface {

class IScreenCapture {
public:
	explicit IScreenCapture() = default;
	virtual ~IScreenCapture() = default;

	virtual Enum::ScreenCaptureType type() const = 0;

	virtual bool capture(ci32 x, ci32 y, ci32 w, ci32 h) = 0;

	virtual ccolor *data() = 0;
};
}
