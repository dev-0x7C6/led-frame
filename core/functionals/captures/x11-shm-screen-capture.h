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
	virtual bool capture(const int32_t x, const int32_t y, const uint32_t w, const uint32_t h) override;
	virtual const uint32_t *data() override;

private:
	std::unique_ptr<Functional::Helper::X11ShmHelper> m_helper;
};
}
}
