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
	virtual int32_t width() override;
	virtual int32_t height() override;

	virtual bool capture() override;
	virtual const uint32_t *data() override;

private:
	QImage m_frame;
};
}
}
