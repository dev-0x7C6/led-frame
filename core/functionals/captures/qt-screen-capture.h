#pragma once

#include <core/interfaces/iscreen-capture.h>
#include <QImage>

namespace Functional {
namespace Capture {

class QtScreenCapture final : public Interface::IScreenCapture {
public:
	explicit QtScreenCapture() = default;
	QtScreenCapture(const QtScreenCapture &) = delete;
	QtScreenCapture(QtScreenCapture &&) noexcept = delete;
	QtScreenCapture &operator=(const QtScreenCapture &) = delete;
	QtScreenCapture &operator=(QtScreenCapture &&) noexcept = delete;
	~QtScreenCapture() final = default;

	Enum::ScreenCaptureType type() const final;

	bool capture(i32 id) final;

	auto data() const noexcept -> const color * final;
	auto width() const noexcept -> u32 final;
	auto height() const noexcept -> u32 final;
	auto bytesPerPixel() const noexcept -> u32 final;

private:
	QImage m_frame;
};
} // namespace Capture
} // namespace Functional
