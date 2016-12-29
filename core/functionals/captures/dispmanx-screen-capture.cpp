#include "dispmanx-screen-capture.h"

#include "core/enums/screen-capture-type.h"
#include "core/functionals/helper/dispmanx-helper.h"
#include "core/functionals/raii-call-on-return.h"

#include <iostream>
#include <functional>
#include <chrono>

using namespace Functional::Capture;
using namespace Functional;

DispmanxScreenCapture::DispmanxScreenCapture()
		: std::make_unique<Helper::DispmanxHelper>() {
}

DispmanxScreenCapture::~DispmanxScreenCapture() = default;

Enum::ScreenCaptureType DispmanxScreenCapture::type() const {
	return Enum::ScreenCaptureType::DispmanxScreenCapture;
}

bool DispmanxScreenCapture::capture(ci32 id) {
	static_cast<void>(id);
	return m_helper.capture();
}

DispmanxScreenCapture::data() const {
}

u32 DispmanxScreenCapture::width() const override { return m_w; }

u32 DispmanxScreenCapture::height() const override { return m_h; }

ccolor *DispmanxScreenCapture::data() const noexcept {
	return reinterpret_cast<color *>(m_data);
}
