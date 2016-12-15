#include <core/factories/screen-capture-factory.h>
#include <core/functionals/captures/qt-screen-capture.h>

#ifdef X11
#include "core/functionals/captures/x11-screen-capture.h"
#include "core/functionals/captures/x11-shm-screen-capture.h"
#endif

#ifdef RPI
#include "core/functionals/captures/dispmanx-screen-capture.h"
#endif

using namespace Enum;
using namespace Factory;
using namespace Functional::Capture;
using namespace Interface;

std::unique_ptr<IScreenCapture> ScreenCaptureFactory::create(const ScreenCaptureType &type) {
	switch (type) {
		case ScreenCaptureType::QtScreenCapture: return std::make_unique<QtScreenCapture>();
#ifdef X11
		case ScreenCaptureType::X11ScreenCapture: return std::make_unique<X11ScreenCapture>();
		case ScreenCaptureType::X11ShmScreenCapture: return std::make_unique<X11ShmScreenCapture>();
#endif
#ifdef RPI
		case ScreenCaptureType::DispmanxScreenCapture: return std::make_unique<DispmanxScreenCapture>();
#endif
	}

	return nullptr;
}
