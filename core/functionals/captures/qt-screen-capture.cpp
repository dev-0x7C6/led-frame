#include <core/functionals/captures/qt-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

using namespace Enum;
using namespace Functional::Capture;

ScreenCaptureType QtScreenCapture::type() const { return ScreenCaptureType::QtScreenCapture; }

bool QtScreenCapture::capture(ci32 x, ci32 y, ci32 w, ci32 h) {
	m_frame = QGuiApplication::screens().first()->grabWindow(0, x, y, static_cast<int>(w), static_cast<int>(h)).toImage();
	return !m_frame.isNull();
}

ccolor *QtScreenCapture::data() { return reinterpret_cast<ccolor *>(m_frame.bits()); }
