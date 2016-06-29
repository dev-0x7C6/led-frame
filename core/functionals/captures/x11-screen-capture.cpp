#include <core/functionals/captures/x11-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

#include <core/functionals/captures/x11-helper.h>

using namespace Enum;
using namespace Functional::Capture;
using namespace Functional::Helper;

X11ScreenCapture::X11ScreenCapture() : m_helper(std::make_unique<X11Helper>()) {}
X11ScreenCapture::~X11ScreenCapture() {}

ScreenCaptureType X11ScreenCapture::type() const { return ScreenCaptureType::X11ScreenCapture; }
int32_t X11ScreenCapture::width() { return m_helper->width(); }
int32_t X11ScreenCapture::height() { return m_helper->height(); }
const uint32_t *X11ScreenCapture::data() { return m_helper->data(); }

// FIXME: X11ScreenCapture capture hardcoded for 1920x1080

bool X11ScreenCapture::capture() { return m_helper->capture(0, 0, 1920, 1080); }

