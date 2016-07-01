#include <core/functionals/captures/qt-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

using namespace Enum;
using namespace Functional::Capture;

ScreenCaptureType QtScreenCapture::type() const { return ScreenCaptureType::QtScreenCapture; }

bool QtScreenCapture::capture(const int32_t x, const int32_t y, const uint32_t w, const uint32_t h) {
	m_frame = QGuiApplication::screens().first()->grabWindow(0, x, y, static_cast<int>(w), static_cast<int>(h)).toImage();
	return !m_frame.isNull();
}

const uint32_t *QtScreenCapture::data() { return reinterpret_cast<const uint32_t *>(m_frame.bits()); }
