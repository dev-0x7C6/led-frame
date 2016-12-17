#pragma once

#include "core/interfaces/iscreen-capture.h"
#include "core/consts.h"
#include "bcm_host.h"
#include "interface/vmcs_host/vcgencmd.h"

namespace Functional {
namespace Capture {

class DispmanxScreenCapture final : public Interface::IScreenCapture {
public:
	explicit DispmanxScreenCapture();
	virtual ~DispmanxScreenCapture();

	virtual Enum::ScreenCaptureType type() const override;
	virtual bool capture(ci32 x, ci32 y, ci32 w, ci32 h) override;
	virtual ccolor *data() override;

private:
	DISPMANX_DISPLAY_HANDLE_T m_display = 0;
	DISPMANX_RESOURCE_HANDLE_T m_resource = 0;
	void *m_data = nullptr;
	i32 m_size = 0;
	i32 m_w = 0;
	i32 m_h = 0;
};
}
}
