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

bool X11ShmHelper::capture(const i32 x, const i32 y, const i32 w, const i32 h) {

	if (!(m_x == x && m_y == y && m_w == w && m_h == h)) {
		cleanup();

		m_frame = XShmCreateImage(m_display, nullptr, DisplayPlanes(m_display, 0), ZPixmap, nullptr, &m_shminfo, w, h);
		const auto size = m_frame->bytes_per_line * m_frame->height;
		m_shminfo.shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0777);
		m_shminfo.shmaddr = m_frame->data = reinterpret_cast<char *>(shmat(m_shminfo.shmid, nullptr, 0));
		m_shminfo.readOnly = false;
		XShmAttach(m_display, &m_shminfo);
		shmctl(m_shminfo.shmid, IPC_RMID, nullptr);

		m_x = x;
		m_y = y;
		m_w = w;
		m_h = h;
	}

	XSync(m_display, false);
	XShmGetImage(m_display, RootWindow(m_display, 0), m_frame, x, y, AllPlanes);
	return m_frame != nullptr;
}

const color *X11ShmHelper::data() { return reinterpret_cast<const color *>(m_frame->data); }

void X11ShmHelper::cleanup() {
	if (!m_frame)
		return;

	XShmDetach(m_display, &m_shminfo);
	XDestroyImage(m_frame);
	shmdt(m_shminfo.shmaddr);
}
