#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <QImage>

namespace Functional {
namespace Capture {

class QtScreenCapture final : public Interface::IScreenCapture {
public:
	explicit QtScreenCapture() = default;
	virtual ~QtScreenCapture() = default;

	virtual Enum::ScreenCaptureType type() const override;

	virtual bool capture(ci32 id) override;

	virtual auto data() const noexcept -> ccolor * override;
	virtual auto width() const noexcept -> u32 override;
	virtual auto height() const noexcept -> u32 override;
	virtual auto bytesPerPixel() const noexcept -> u32 override;

private:
	QImage m_frame;
};
} // namespace Capture
} // namespace Functional
