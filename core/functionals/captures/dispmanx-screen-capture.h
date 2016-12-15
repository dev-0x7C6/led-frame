#pragma once

#include "core/interfaces/iscreen-capture.h"
#include "core/consts.h"

namespace Functional {
namespace Capture {

class DispmanxScreenCapture final : public Interface::IScreenCapture {
public:
	explicit DispmanxScreenCapture();

	virtual Enum::ScreenCaptureType type() const override;
	virtual bool capture(ci32 x, ci32 y, ci32 w, ci32 h) override;
	virtual ccolor *data() override;

private:
	ccolor *m_data = nullptr;
	i32 m_w = 0;
	i32 m_h = 0;
};
}
}
