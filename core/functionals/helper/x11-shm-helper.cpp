#include <core/functionals/helper/x11-shm-helper.h>

using namespace Functional::Helper;

X11ShmHelper::X11ShmHelper()
		: m_display(XOpenDisplay(nullptr))
		, m_frame(nullptr)
{
	// FIXME: X11ShmHelper capture hardcoded for 1920x1080
	m_frame = XShmCreateImage(m_display, 0, DisplayPlanes(m_display, 0), ZPixmap, 0, &m_shminfo, 1920, 1080);

	const auto size = m_frame->bytes_per_line * m_frame->height;

	m_shminfo.shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0777);
	m_shminfo.shmaddr = m_frame->data = reinterpret_cast<char*>(shmat(m_shminfo.shmid, 0, 0));
	m_shminfo.readOnly = false;

	XShmAttach(m_display, &m_shminfo);

}

X11ShmHelper::~X11ShmHelper() { destroyFrame(); }

bool X11ShmHelper::capture() {
	XShmGetImage(m_display, RootWindow(m_display, 0), m_frame, 0, 0, AllPlanes);
	return m_frame != nullptr;
}

int32_t X11ShmHelper::width() const { return m_frame->width; }
int32_t X11ShmHelper::height() const { return m_frame->height; }

const uint32_t *X11ShmHelper::data() { return reinterpret_cast<uint32_t*>(m_frame->data); }

void X11ShmHelper::destroyFrame() {
	if (m_frame)
		XDestroyImage(m_frame);
}
