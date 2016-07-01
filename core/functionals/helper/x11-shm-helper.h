#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <cstdint>

namespace Functional {
namespace Helper {

class X11ShmHelper {
public:
	explicit X11ShmHelper();
	virtual ~X11ShmHelper();

	bool capture(const int32_t x, const int32_t y, const uint32_t w, const uint32_t h);
	const uint32_t *data();

private:
	void cleanup();

private:
	XShmSegmentInfo m_shminfo;
	Display *m_display;
	XImage *m_frame;

	int32_t m_x;
	int32_t m_y;
	uint32_t m_w;
	uint32_t m_h;
};
}
}
