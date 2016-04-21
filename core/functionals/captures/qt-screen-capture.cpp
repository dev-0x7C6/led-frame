#include <core/functionals/captures/qt-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

using namespace Enum;
using namespace Functional::Capture;

ScreenCaptureType QtScreenCapture::type() const {
	return ScreenCaptureType::QtScreenCapture;
}

uint32_t QtScreenCapture::width() {
	return m_frame.width();
}

uint32_t QtScreenCapture::height() {
	return m_frame.height();
}

void QtScreenCapture::capture() {
	m_frame = QGuiApplication::screens().first()->grabWindow(0).toImage();
}

const uint32_t *QtScreenCapture::data() {
	return reinterpret_cast<const uint32_t *>(m_frame.bits());
}
