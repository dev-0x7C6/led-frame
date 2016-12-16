#include "dispmanx-screen-capture.h"
#include "core/functionals/raii-call-on-return.h"
#include "core/enums/screen-capture-type.h"

#include <iostream>
#include <functional>
#include <chrono>

using namespace Functional::Capture;
using namespace Functional;

#ifndef ALIGN_TO_16
#define ALIGN_TO_16(x) ((x + 15) & ~15)
#endif

DispmanxScreenCapture::DispmanxScreenCapture() {
	m_display = vc_dispmanx_display_open(0);

	DISPMANX_MODEINFO_T modeInfo;
	vc_dispmanx_display_get_info(m_display, &modeInfo);

	m_w = modeInfo.width;
	m_h = modeInfo.height;

	auto scanline = 3 * ALIGN_TO_16(m_w);

	m_data = static_cast<ccolor *>(std::malloc(scanline * m_h));
}

Enum::ScreenCaptureType DispmanxScreenCapture::type() const {
	return Enum::ScreenCaptureType::DispmanxScreenCapture;
}

bool DispmanxScreenCapture::capture(ci32 x, ci32 y, ci32 w, ci32 h) {
	static_cast<void>(x);
	static_cast<void>(y);
	static_cast<void>(h);
	static_cast<void>(w);

	VC_IMAGE_TYPE_T imageType = VC_IMAGE_RGB888;

	int scanline = 3 * ALIGN_TO_16(m_w);

	void *dmxImagePtr = malloc(scanline * m_h);

	if (dmxImagePtr == NULL) {
		exit(EXIT_FAILURE);
	}

	uint32_t vcImagePtr = 0;
	DISPMANX_RESOURCE_HANDLE_T resourceHandle;
	resourceHandle = vc_dispmanx_resource_create(imageType, m_w, m_h, &vcImagePtr);

	RaiiCallOnReturn resourceFree = {[&]() { vc_dispmanx_resource_delete(resourceHandle); }};

	vc_dispmanx_snapshot(m_display, resourceHandle, DISPMANX_NO_ROTATE);

	VC_RECT_T rect;
	vc_dispmanx_rect_set(&rect, 0, 0, m_w, m_h);

	vc_dispmanx_resource_read_data(resourceHandle, &rect, dmxImagePtr, scanline);

	return 0;
}

ccolor *DispmanxScreenCapture::data() {
	return m_data;
}
