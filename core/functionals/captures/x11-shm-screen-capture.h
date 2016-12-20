#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <memory>

namespace Functional {
namespace Helper {
class X11ShmHelper;
}
}

namespace Functional {
namespace Capture {

class X11ShmScreenCapture : public Interface::IScreenCapture {
public:
	explicit X11ShmScreenCapture();
	virtual ~X11ShmScreenCapture();

	virtual Enum::ScreenCaptureType type() const override;
	virtual bool capture(ci32 id) override;
	virtual auto data() const noexcept -> ccolor * override;
	virtual auto width() const noexcept -> u32 override { return m_w; }
	virtual auto height() const noexcept -> u32 override { return m_h; }
	virtual auto bytesPerPixel() const noexcept -> u32 override { return m_bpp; }

private:
	std::unique_ptr<Functional::Helper::X11ShmHelper> m_helper;
	u32 m_w = 0;
	u32 m_h = 0;
	u32 m_bpp = 4;
};
}
}
