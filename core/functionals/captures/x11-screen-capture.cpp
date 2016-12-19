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
ccolor *X11ScreenCapture::data() { return m_helper->data(); }

bool X11ScreenCapture::capture(ci32 id) {
	if (QGuiApplication::screens().count() <= id)
		return false;

	auto rect = QGuiApplication::screens().at(id)->geometry();
	m_w = rect.width();
	m_h = rect.height();

	return m_helper->capture(rect.x(), rect.y(), m_w, m_h);
}
