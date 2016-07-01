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

	virtual bool capture(const int32_t x, const int32_t y, const uint32_t w, const uint32_t h) override;
	virtual const uint32_t *data() override;

private:
	QImage m_frame;
};
}
}
