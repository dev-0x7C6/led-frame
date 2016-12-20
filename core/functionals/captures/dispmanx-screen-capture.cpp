#include "dispmanx-screen-capture.h"
#include "core/functionals/raii-call-on-return.h"
#include "core/enums/screen-capture-type.h"

#include <iostream>
#include <functional>
#include <chrono>

using namespace Functional::Capture;
using namespace Functional;

DispmanxScreenCapture::DispmanxScreenCapture() {
	m_display = vc_dispmanx_display_open(0);
	if (!m_display) {
		std::cerr << "vc_dispmanx_display_open failed!" << std::endl;
		return;
	}

	DISPMANX_MODEINFO_T modeInfo;
	if (vc_dispmanx_display_get_info(m_display, &modeInfo)) {
		std::cerr << "vc_dispmanx_display_get_info failed!" << std::endl;
		return;
	}

	m_w = modeInfo.width;
	m_h = modeInfo.height;

	m_data = std::malloc(m_w * m_h * 4);
	if (m_data == nullptr) {
		std::cerr << "std::malloc failed!" << std::endl;
		return;
	}

	VC_IMAGE_TYPE_T imageType = VC_IMAGE_RGBA32;
	uint32_t vcImagePtr = 0;

	m_resource = vc_dispmanx_resource_create(imageType, m_w, m_h, &vcImagePtr);
	if (m_resource == 0) {
		std::cerr << "vc_dispmanx_resource_create failed!" << std::endl;
		return;
	}
}

DispmanxScreenCapture::~DispmanxScreenCapture() {
	if (m_data != nullptr)
		std::free(m_data);

	if (m_resource != 0)
		vc_dispmanx_resource_delete(m_resource);

	if (m_display != 0)
		vc_dispmanx_display_close(m_display);
}

Enum::ScreenCaptureType DispmanxScreenCapture::type() const {
	return Enum::ScreenCaptureType::DispmanxScreenCapture;
}

bool DispmanxScreenCapture::capture(ci32 id) {
	static_cast<void>(id);

	if (vc_dispmanx_snapshot(m_display, m_resource, DISPMANX_NO_ROTATE)) {
		std::cerr << "vc_dispmanx_snapshot failed!" << std::endl;
		return false;
	}

	VC_RECT_T rect;
	if (vc_dispmanx_rect_set(&rect, 0, 0, m_w, m_h)) {
		std::cerr << "vc_dispmanx_rect_set failed!" << std::endl;
		return false;
	}

	if (vc_dispmanx_resource_read_data(m_resource, &rect, m_data, m_w * 4)) {
		std::cerr << "vc_dispmanx_resource_read_data failed!" << std::endl;
		return false;
	}

	return true;
}

ccolor *DispmanxScreenCapture::data() const noexcept {
	return reinterpret_cast<color *>(m_data);
}
