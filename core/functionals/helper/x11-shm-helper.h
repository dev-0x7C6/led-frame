#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <core/types.h>

namespace Functional {
namespace Helper {

class X11ShmHelper {
public:
	explicit X11ShmHelper();
	virtual ~X11ShmHelper();

	bool capture(i32 x, i32 y, i32 w, i32 h);
	const color *data();

private:
	void cleanup();

private:
	XShmSegmentInfo m_shminfo;
	Display *m_display;
	XImage *m_frame;

	int m_x;
	int m_y;
	int m_w;
	int m_h;
};
} // namespace Helper
} // namespace Functional
