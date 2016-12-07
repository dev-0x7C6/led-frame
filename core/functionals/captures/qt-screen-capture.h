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

	virtual bool capture(ci32 x, ci32 y, ci32 w, ci32 h) override;
	virtual ccolor *data() override;

private:
	QImage m_frame;
};
}
}
