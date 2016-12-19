#pragma once

#include "core/interfaces/iscreen-capture.h"
#include "core/types.h"
#include "bcm_host.h"
#include "interface/vmcs_host/vcgencmd.h"

namespace Functional {
namespace Capture {

class DispmanxScreenCapture final : public Interface::IScreenCapture {
public:
	explicit DispmanxScreenCapture();
	virtual ~DispmanxScreenCapture();

	virtual Enum::ScreenCaptureType type() const override;
	virtual bool capture(ci32 id) override;
	virtual ccolor *data() override;
	virtual u32 width() const noexcept override { return m_w; }
	virtual u32 height() const noexcept override { return m_h; }

private:
	DISPMANX_DISPLAY_HANDLE_T m_display = 0;
	DISPMANX_RESOURCE_HANDLE_T m_resource = 0;
	void *m_data = nullptr;
	u32 m_size = 0;
	u32 m_w = 0;
	u32 m_h = 0;
};
}
}
