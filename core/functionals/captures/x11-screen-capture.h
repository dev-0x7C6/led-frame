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
	virtual int32_t width() override;
	virtual int32_t height() override;
	virtual bool capture() override;
	virtual const uint32_t *data() override;

private:
	std::unique_ptr<Functional::Helper::X11Helper> m_helper;
};
}
}
