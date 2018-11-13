#pragma once

namespace Enum {

enum class ScreenCaptureType {
	QtScreenCapture,
#ifdef X11
	X11ScreenCapture,
	X11ShmScreenCapture,
#endif
#ifdef RPI
	DispmanxScreenCapture,
#endif
};
} // namespace Enum
