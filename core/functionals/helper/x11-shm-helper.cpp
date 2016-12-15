#include <core/functionals/helper/x11-shm-helper.h>

using namespace Functional::Helper;

X11ShmHelper::X11ShmHelper()
		: m_display(XOpenDisplay(nullptr))
		, m_frame(nullptr)
		, m_x(0)
		, m_y(0)
		, m_w(0)
		, m_h(0) {
}

X11ShmHelper::~X11ShmHelper() { cleanup(); }

bool X11ShmHelper::capture(ci32 x, ci32 y, ci32 w, ci32 h) {

	if (!(m_x == x && m_y == y && m_w == w && m_h == h)) {
		cleanup();

		m_frame = XShmCreateImage(m_display, 0, DisplayPlanes(m_display, 0), ZPixmap, 0, &m_shminfo, w, h);
		const auto size = m_frame->bytes_per_line * m_frame->height;
		m_shminfo.shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0777);
		m_shminfo.shmaddr = m_frame->data = reinterpret_cast<char *>(shmat(m_shminfo.shmid, 0, 0));
		m_shminfo.readOnly = false;
		XShmAttach(m_display, &m_shminfo);
		shmctl(m_shminfo.shmid, IPC_RMID, 0);

		m_x = x;
		m_y = y;
		m_w = w;
		m_h = h;
	}

	XSync(m_display, false);
	XShmGetImage(m_display, RootWindow(m_display, 0), m_frame, x, y, AllPlanes);
	return m_frame != nullptr;
}

ccolor *X11ShmHelper::data() { return reinterpret_cast<ccolor *>(m_frame->data); }

void X11ShmHelper::cleanup() {
	if (!m_frame)
		return;

	XShmDetach(m_display, &m_shminfo);
	XDestroyImage(m_frame);
	shmdt(m_shminfo.shmaddr);
}
