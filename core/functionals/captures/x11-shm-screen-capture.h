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
	virtual bool capture(ci32 x, ci32 y, ci32 w, ci32 h) override;
	virtual ccolor *data() override;

private:
	std::unique_ptr<Functional::Helper::X11ShmHelper> m_helper;
};
}
}
