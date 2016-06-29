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

	bool capture();
	int32_t width() const;
	int32_t height() const;
	const uint32_t *data();

private:
	void destroyFrame();

private:
	XShmSegmentInfo m_shminfo;
	Display *m_display;
	XImage *m_frame;
};

}
}
