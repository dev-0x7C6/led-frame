#include <core/factories/screen-capture-factory.h>
#include <core/functionals/captures/qt-screen-capture.h>

#ifdef X11
#include <core/functionals/captures/x11-screen-capture.h>
#endif

using namespace Enum;
using namespace Factory;
using namespace Functional::Capture;
using namespace Interface;

std::unique_ptr<IScreenCapture> ScreenCaptureFactory::create(const ScreenCaptureType &type) {
	switch (type) {
		case ScreenCaptureType::QtScreenCapture:
			return std::make_unique<QtScreenCapture>();
#ifdef X11
		case ScreenCaptureType::X11ScreenCapture:
			return std::make_unique<X11ScreenCapture>();
#endif
	}

	return nullptr;
}
