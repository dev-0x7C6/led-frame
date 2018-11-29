#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <memory>

namespace Functional {
namespace Helper {
class X11Helper;
}
} // namespace Functional

namespace Functional {
namespace Capture {

class X11ScreenCapture final : public Interface::IScreenCapture {
public:
	explicit X11ScreenCapture();
	X11ScreenCapture(const X11ScreenCapture &) = delete;
	X11ScreenCapture(X11ScreenCapture &&) noexcept = delete;
	X11ScreenCapture &operator=(const X11ScreenCapture &) = delete;
	X11ScreenCapture &operator=(X11ScreenCapture &&) noexcept = delete;
	~X11ScreenCapture() final;

	Enum::ScreenCaptureType type() const final;
	bool capture(ci32 id) final;

	auto data() const noexcept -> ccolor * final;
	auto width() const noexcept -> u32 final { return m_w; }
	auto height() const noexcept -> u32 final { return m_h; }
	auto bytesPerPixel() const noexcept -> u32 final { return m_bpp; }

private:
	std::unique_ptr<Functional::Helper::X11Helper> m_helper;
	u32 m_w = 0;
	u32 m_h = 0;
	u32 m_bpp = 4;
};
} // namespace Capture
} // namespace Functional
