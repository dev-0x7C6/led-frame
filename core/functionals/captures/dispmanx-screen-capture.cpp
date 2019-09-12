#include "dispmanx-screen-capture.h"

#include "core/enums/screen-capture-type.h"
#include "core/functionals/helper/dispmanx-helper.h"

#include <iostream>
#include <functional>
#include <chrono>

using namespace Functional::Helper;
using namespace Functional::Capture;
using namespace Functional;

DispmanxScreenCapture::DispmanxScreenCapture()
		: m_helper(std::make_unique<DispmanxHelper>()) {
}

DispmanxScreenCapture::~DispmanxScreenCapture() = default;

Enum::ScreenCaptureType DispmanxScreenCapture::type() const {
	return Enum::ScreenCaptureType::DispmanxScreenCapture;
}

bool DispmanxScreenCapture::capture(const i32 id) {
	static_cast<void>(id);
	return m_helper->capture();
}

auto DispmanxScreenCapture::width() const noexcept -> u32 { return m_helper->width(); }

auto DispmanxScreenCapture::height() const noexcept -> u32 { return m_helper->height(); }

auto DispmanxScreenCapture::data() const noexcept -> const color * {
	return reinterpret_cast<color *>(m_helper->data());
}
