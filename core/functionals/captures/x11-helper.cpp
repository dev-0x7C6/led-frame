#include <core/functionals/captures/x11-helper.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace Functional::Helper;

class x11_display_helper {
public:
	x11_display_helper(const char *name = nullptr)
			: m_display(XOpenDisplay(name)) {
	}

	auto rootWindow() const noexcept {
		return DefaultRootWindow(m_display);
	}

	auto is_valid() const noexcept {
		return m_display != nullptr;
	}

	auto display() const noexcept {
		return m_display;
	}

	~x11_display_helper() noexcept {
		if (m_display)
			XCloseDisplay(m_display);
	}

private:
	Display *m_display{nullptr};
};

class x11_image_helper {
public:
	x11_image_helper(const std::unique_ptr<x11_display_helper> &helper, i32 x, i32 y, i32 w, i32 h) {
		if (helper && helper->is_valid())
			m_image = XGetImage(helper->display(), helper->rootWindow(), x, y, static_cast<u32>(w), static_cast<u32>(h), AllPlanes, ZPixmap);
	}

	const color *data() const noexcept {
		if (m_image)
			return reinterpret_cast<const color *>(m_image->data);

		return nullptr;
	}

	auto is_valid() const noexcept {
		return data() != nullptr;
	}

	~x11_image_helper() noexcept {
		if (m_image)
			XDestroyImage(m_image);
	}

private:
	XImage *m_image{nullptr};
};

X11Helper::X11Helper() noexcept
		: m_display(std::make_unique<x11_display_helper>()) {
}

bool X11Helper::is_open() const noexcept {
	return m_display != nullptr;
}

X11Helper::~X11Helper() = default;

bool X11Helper::capture(const i32 x, const i32 y, const i32 w, const i32 h) {
	m_frame = std::make_unique<x11_image_helper>(m_display, x, y, w, h);
	return m_frame->is_valid();
}

const color *X11Helper::data() noexcept {
	if (m_frame && m_frame->is_valid())
		return m_frame->data();

	return nullptr;
}
