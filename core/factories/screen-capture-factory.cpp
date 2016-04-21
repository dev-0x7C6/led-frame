#include <core/factories/screen-capture-factory.h>
#include <core/functionals/captures/qt-screen-capture.h>
#include <core/functionals/captures/x11-screen-capture.h>

using namespace Enum;
using namespace Factory;
using namespace Functional::Capture;
using namespace Interface;

std::unique_ptr<IScreenCapture> ScreenCaptureFactory::create(const ScreenCaptureType &type) {
	switch (type) {
		case ScreenCaptureType::QtScreenCapture:
			return std::make_unique<QtScreenCapture>();

		case ScreenCaptureType::X11ScreenCapture:
			return std::make_unique<X11ScreenCapture>();
	}

	return nullptr;
}
