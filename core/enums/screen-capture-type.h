#pragma once

namespace Enum {

enum class ScreenCaptureType {
	QtScreenCapture,
#ifdef X11
	X11ScreenCapture,
#endif
};
}
