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
X11ShmScreenCapture::~X11ShmScreenCapture() {}

ScreenCaptureType X11ShmScreenCapture::type() const { return ScreenCaptureType::X11ShmScreenCapture; }
bool X11ShmScreenCapture::capture(const int32_t x, const int32_t y, const uint32_t w, const uint32_t h) { return m_helper->capture(x, y, w, h); }
const uint32_t *X11ShmScreenCapture::data() { return m_helper->data(); }
