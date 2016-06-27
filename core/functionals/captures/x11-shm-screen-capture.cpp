#include <core/functionals/captures/x11-shm-screen-capture.h>

using namespace Enum;
using namespace Functional::Capture;

//ftp://ftp.mcs.vuw.ac.nz/_mnt/X/R5/mit/extensions/test/shmtest.c

X11ShmScreenCapture::X11ShmScreenCapture()
		: m_frame(nullptr)

{
	m_display = XOpenDisplay(nullptr);
	m_root = DefaultRootWindow(m_display);

	m_frame = XShmCreateImage(m_display, 0, DisplayPlanes(m_display, 0), ZPixmap,
									  0, &shminfo, 1366, 768);
	shminfo.shmid=shmget (IPC_PRIVATE,m_frame->bytes_per_line*m_frame->height,IPC_CREAT|0777);
	shminfo.shmaddr= m_frame->data = reinterpret_cast<char*>(shmat (shminfo.shmid, 0, 0));
	shminfo.readOnly = False;

	XShmAttach(m_display,&shminfo);
}

X11ShmScreenCapture::~X11ShmScreenCapture() {
	if (m_frame)
		XDestroyImage(m_frame);

	XCloseDisplay(m_display);
}

ScreenCaptureType X11ShmScreenCapture::type() const {
	return ScreenCaptureType::X11ShmScreenCapture;
}

uint32_t X11ShmScreenCapture::width() {
	return m_frame->width;
}

uint32_t X11ShmScreenCapture::height() {
	return m_frame->height;
}

void X11ShmScreenCapture::capture(int x, int y, int, int) {
//	if (m_frame)
//		XDestroyImage(m_frame);

	XShmGetImage(m_display, RootWindow(m_display, 0), m_frame, x, y, AllPlanes);
}

const uint32_t *X11ShmScreenCapture::data() {
	return reinterpret_cast<const uint32_t *>(shminfo.shmaddr);
}
