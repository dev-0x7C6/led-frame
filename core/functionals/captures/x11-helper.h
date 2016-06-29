#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>

namespace Functional {
namespace Helper {

class X11Helper {
public:
	explicit X11Helper();
	virtual ~X11Helper();

	bool capture(int x, int y, int w, int h);
	int32_t width() const;
	int32_t height() const;
	const uint32_t *data();

private:
	void destroyFrame();

private:
	Display *m_display;
	Window m_window;
	XImage *m_frame;
};
}
}
