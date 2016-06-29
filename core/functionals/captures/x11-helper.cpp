#include <core/functionals/captures/x11-helper.h>
using namespace Functional::Helper;

X11Helper::X11Helper()
		: m_display(XOpenDisplay(nullptr))
		, m_window(DefaultRootWindow(m_display))
		, m_frame(nullptr) {}

X11Helper::~X11Helper() { destroyFrame(); }

bool X11Helper::capture(int x, int y, int w, int h) {
	destroyFrame();
	m_frame = XGetImage(m_display, m_window, x, y, w, h, AllPlanes, ZPixmap);
	return m_frame != nullptr;
}

int32_t X11Helper::width() const { return m_frame->width; }
int32_t X11Helper::height() const { return m_frame->height; }

const uint32_t *X11Helper::data() { return reinterpret_cast<uint32_t*>(m_frame->data); }

void X11Helper::destroyFrame() {
	if (m_frame)
		XDestroyImage(m_frame);
}
