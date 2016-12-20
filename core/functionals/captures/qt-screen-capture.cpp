#include <core/functionals/captures/qt-screen-capture.h>

#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

using namespace Enum;
using namespace Functional::Capture;

ScreenCaptureType QtScreenCapture::type() const { return ScreenCaptureType::QtScreenCapture; }

bool QtScreenCapture::capture(ci32 id) {
	if (QGuiApplication::screens().count() <= id)
		return false;

	auto rect = QGuiApplication::screens().first()->geometry();

	m_frame = QGuiApplication::screens().first()->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height()).toImage();
	return !m_frame.isNull();
}

ccolor *QtScreenCapture::data() const noexcept { return reinterpret_cast<ccolor *>(m_frame.bits()); }

u32 QtScreenCapture::width() const noexcept {
	return static_cast<u32>(m_frame.width());
}

u32 QtScreenCapture::height() const noexcept {
	return static_cast<u32>(m_frame.height());
}

u32 QtScreenCapture::bytesPerPixel() const noexcept {
	return m_frame.depth();
}
