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

	virtual ccolor *data() override;
	virtual u32 width() const noexcept override;
	virtual u32 height() const noexcept override;

private:
	QImage m_frame;
};
}
}
