#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <core/types.h>

namespace Functional {
namespace Helper {

class X11Helper {
public:
	explicit X11Helper() noexcept;
	~X11Helper();

	bool capture(i32 x, i32 y, i32 w, i32 h);
	const color *data() noexcept;

private:
	void destroyFrame();

private:
	Display *m_display;
	Window m_window;
	XImage *m_frame;
};
} // namespace Helper
} // namespace Functional
