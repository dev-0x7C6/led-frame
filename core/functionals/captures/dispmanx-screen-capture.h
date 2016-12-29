#pragma once

#include "core/interfaces/iscreen-capture.h"
#include "core/types.h"
#include <memory>

namespace Functional {
namespace Helper {
class DispmanxHelper;
}
namespace Capture {

class DispmanxScreenCapture final : public Interface::IScreenCapture {
public:
	explicit DispmanxScreenCapture();
	virtual ~DispmanxScreenCapture();

	virtual Enum::ScreenCaptureType type() const override;
	virtual bool capture(ci32 id) override;
	virtual auto data() const noexcept -> ccolor * override;
	virtual auto width() const noexcept -> u32;
	virtual auto height() const noexcept -> u32;
	virtual auto bytesPerPixel() const noexcept -> u32 override { return 4; }

private:
	void *m_data = nullptr;
	std::unique_ptr<Helper::DispmanxHelper> m_helper;
	u32 m_size = 0;
	u32 m_w = 0;
	u32 m_h = 0;
};
}
}
