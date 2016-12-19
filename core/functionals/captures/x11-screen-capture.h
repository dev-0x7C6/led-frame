#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <memory>

namespace Functional {
namespace Helper {
class X11Helper;
}
}

namespace Functional {
namespace Capture {

class X11ScreenCapture final : public Interface::IScreenCapture {
public:
	explicit X11ScreenCapture();
	virtual ~X11ScreenCapture();

	virtual Enum::ScreenCaptureType type() const override;
	virtual bool capture(ci32 id) override;
	virtual ccolor *data() override;
	virtual u32 width() const noexcept override { return m_w; }
	virtual u32 height() const noexcept override { return m_h; }

private:
	std::unique_ptr<Functional::Helper::X11Helper> m_helper;
	u32 m_w = 0;
	u32 m_h = 0;
};
}
}
