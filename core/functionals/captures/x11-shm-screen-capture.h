#pragma once

#include <core/interfaces/iscreen-capture.h>

#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

namespace Functional {
namespace Capture {

class X11ShmScreenCapture : public Interface::IScreenCapture {
public:
	explicit X11ShmScreenCapture();
	virtual ~X11ShmScreenCapture();

	virtual Enum::ScreenCaptureType type() const override;
	virtual uint32_t width() override;
	virtual uint32_t height() override;
	virtual void capture(int x, int y, int w, int h) override;
	virtual const uint32_t *data() override;

private:
	XShmSegmentInfo shminfo;

	Display *m_display;
	Window m_root;
	XImage *m_frame;
};

}
}
