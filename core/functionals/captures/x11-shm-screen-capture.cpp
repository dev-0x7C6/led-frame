#include <core/functionals/captures/x11-shm-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

#include <core/functionals/helper/x11-shm-helper.h>

using namespace Enum;
using namespace Functional::Capture;
using namespace Functional::Helper;

X11ShmScreenCapture::X11ShmScreenCapture() : m_helper(std::make_unique<X11ShmHelper>()) {}
X11ShmScreenCapture::~X11ShmScreenCapture() {}

ScreenCaptureType X11ShmScreenCapture::type() const {
	return ScreenCaptureType::X11ShmScreenCapture;
}

// TODO: Recreate XImage when screen settings changed

int32_t X11ShmScreenCapture::width() {
	return m_helper->width();
}

int32_t X11ShmScreenCapture::height() {
	return m_helper->height();
}

bool X11ShmScreenCapture::capture() {
	return m_helper->capture();
}

const uint32_t *X11ShmScreenCapture::data() {
	return m_helper->data();
}
