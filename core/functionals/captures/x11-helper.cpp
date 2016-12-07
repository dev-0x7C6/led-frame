#include <core/functionals/captures/x11-helper.h>
using namespace Functional::Helper;

X11Helper::X11Helper()
		: m_display(XOpenDisplay(nullptr))
		, m_window(DefaultRootWindow(m_display))
		, m_frame(nullptr) {}

X11Helper::~X11Helper() { destroyFrame(); }

bool X11Helper::capture(ci32 x, ci32 y, ci32 w, ci32 h) {
	destroyFrame();
	m_frame = XGetImage(m_display, m_window, x, y, w, h, AllPlanes, ZPixmap);
	return m_frame != nullptr;
}

ccolor *X11Helper::data() { return reinterpret_cast<ccolor *>(m_frame->data); }

void X11Helper::destroyFrame() {
	if (m_frame)
		XDestroyImage(m_frame);
}
