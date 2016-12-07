#include <core/functionals/captures/x11-shm-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

#include <core/functionals/helper/x11-shm-helper.h>

using namespace Enum;
using namespace Functional::Capture;
using namespace Functional::Helper;

X11ShmScreenCapture::X11ShmScreenCapture()
		: m_helper(std::make_unique<X11ShmHelper>()) {}

X11ShmScreenCapture::~X11ShmScreenCapture() = default;

ScreenCaptureType X11ShmScreenCapture::type() const { return ScreenCaptureType::X11ShmScreenCapture; }
bool X11ShmScreenCapture::capture(ci32 x, ci32 y, ci32 w, ci32 h) { return m_helper->capture(x, y, w, h); }
ccolor *X11ShmScreenCapture::data() { return m_helper->data(); }
