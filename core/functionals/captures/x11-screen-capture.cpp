#include <core/functionals/captures/x11-screen-capture.h>

using namespace Enum;
using namespace Functional::Capture;

X11ScreenCapture::X11ScreenCapture()
		: m_frame(nullptr)

{
	m_display = XOpenDisplay(nullptr);
	m_root = DefaultRootWindow(m_display);
}

X11ScreenCapture::~X11ScreenCapture() {
	if (m_frame)
		XDestroyImage(m_frame);

	XCloseDisplay(m_display);
}

ScreenCaptureType X11ScreenCapture::type() const {
	return ScreenCaptureType::X11ScreenCapture;
}

uint32_t X11ScreenCapture::width() {
	return m_frame->width;
}

uint32_t X11ScreenCapture::height() {
	return m_frame->height;
}

void X11ScreenCapture::capture(int x, int y, int w, int h) {
	if (m_frame)
		XDestroyImage(m_frame);

	m_frame = XGetImage(m_display, m_root, x, y, w, h, AllPlanes, ZPixmap);
}

const uint32_t *X11ScreenCapture::data() {
	return reinterpret_cast<const uint32_t *>(m_frame->data);
}
