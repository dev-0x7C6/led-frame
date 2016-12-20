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

	bool capture(ci32 x, ci32 y, ci32 w, ci32 h);
	ccolor *data();

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
}
}
