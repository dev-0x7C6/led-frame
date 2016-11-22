#include <core/functionals/captures/x11-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

#include <core/functionals/captures/x11-helper.h>

using namespace Enum;
using namespace Functional::Capture;
using namespace Functional::Helper;

X11ScreenCapture::X11ScreenCapture()
		: m_helper(std::make_unique<X11Helper>()) {}

X11ScreenCapture::~X11ScreenCapture() = default;

ScreenCaptureType X11ScreenCapture::type() const { return ScreenCaptureType::X11ScreenCapture; }
const uint32_t *X11ScreenCapture::data() { return m_helper->data(); }

bool X11ScreenCapture::capture(const int32_t x, const int32_t y, const uint32_t w, const uint32_t h) { return m_helper->capture(x, y, w, h); }
