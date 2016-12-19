#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "core/types.h"

namespace Functional {
namespace Helper {

class X11Helper {
public:
	explicit X11Helper();
	virtual ~X11Helper();

	bool capture(ci32 x, ci32 y, ci32 w, ci32 h);
	ccolor *data();

private:
	void destroyFrame();

private:
	Display *m_display;
	Window m_window;
	XImage *m_frame;
};
}
}
