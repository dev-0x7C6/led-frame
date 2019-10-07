#pragma once

#include <core/types.h>
#include <memory>

class x11_display_helper;
class x11_image_helper;

namespace Functional {
namespace Helper {

class X11Helper {
public:
	explicit X11Helper() noexcept;
	~X11Helper();

	bool is_open() const noexcept;
	bool capture(i32 x, i32 y, i32 w, i32 h);
	const color *data() noexcept;

private:
	std::unique_ptr<x11_display_helper> m_display;
	std::unique_ptr<x11_image_helper> m_frame;
};
} // namespace Helper
} // namespace Functional
