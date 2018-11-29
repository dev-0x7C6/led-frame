#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <memory>

namespace Functional {
namespace Helper {
class X11ShmHelper;
}
} // namespace Functional

namespace Functional {
namespace Capture {

class X11ShmScreenCapture final : public Interface::IScreenCapture {
public:
	explicit X11ShmScreenCapture();
	X11ShmScreenCapture(const X11ShmScreenCapture &) = delete;
	X11ShmScreenCapture(X11ShmScreenCapture &&) noexcept = delete;
	X11ShmScreenCapture &operator=(const X11ShmScreenCapture &) = delete;
	X11ShmScreenCapture &operator=(X11ShmScreenCapture &&) noexcept = delete;
	~X11ShmScreenCapture() final;

	Enum::ScreenCaptureType type() const final;
	bool capture(ci32 id) final;
	auto data() const noexcept -> ccolor * final;
	auto width() const noexcept -> u32 final { return m_w; }
	auto height() const noexcept -> u32 final { return m_h; }
	auto bytesPerPixel() const noexcept -> u32 final { return m_bpp; }

private:
	std::unique_ptr<Functional::Helper::X11ShmHelper> m_helper;
	u32 m_w = 0;
	u32 m_h = 0;
	u32 m_bpp = 4;
};
} // namespace Capture
} // namespace Functional
