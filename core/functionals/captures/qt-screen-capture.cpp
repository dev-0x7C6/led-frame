#include <core/functionals/captures/qt-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

using namespace Enum;
using namespace Functional::Capture;

ScreenCaptureType QtScreenCapture::type() const {
	return ScreenCaptureType::QtScreenCapture;
}

int32_t QtScreenCapture::width() {
	return m_frame.width();
}

int32_t QtScreenCapture::height() {
	return m_frame.height();
}

bool QtScreenCapture::capture() {
	m_frame = QGuiApplication::screens().first()->grabWindow(0).toImage();
	return !m_frame.isNull();
}

const uint32_t *QtScreenCapture::data() {
	return reinterpret_cast<const uint32_t *>(m_frame.bits());
}
