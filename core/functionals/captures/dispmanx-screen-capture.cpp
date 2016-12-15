#include "dispmanx-screen-capture.h"
#include "core/functionals/raii-call-on-return.h"
#include "core/enums/screen-capture-type.h"

#include <functional>
#include <interface/vmcs_host/vcgencmd.h>

using namespace Functional::Capture;
using namespace Functional;

#ifndef ALIGN_TO_16
#define ALIGN_TO_16(x) ((x + 15) & ~15)
#endif

DispmanxScreenCapture::DispmanxScreenCapture() {
	auto display = vc_dispmanx_display_open(0);

	DISPMANX_MODEINFO_T modeInfo;
	vc_dispmanx_display_get_info(display, &modeInfo);

	m_w = modeInfo.width;
	m_h = modeInfo.height;

	auto scanline = 3 * ALIGN_TO_16(m_w);

	m_data = static_cast<ccolor *>(std::malloc(scanline * m_h))
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

	void *dmxImagePtr = malloc(scanline * height);

	if (dmxImagePtr == NULL) {
		kprintf("unable to allocated image buffer\n");
		exit(EXIT_FAILURE);
	}

	uint32_t vcImagePtr = 0;
	DISPMANX_RESOURCE_HANDLE_T resourceHandle;
	resourceHandle = vc_dispmanx_resource_create(imageType, width, height, &vcImagePtr);

	RaiiCallOnReturn resourceFree = {[&]() { vc_dispmanx_resource_delete(resourceHandle); }};

	auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 300000; ++i) {
		result = vc_dispmanx_snapshot(displayHandle, resourceHandle, DISPMANX_NO_ROTATE);

		if (result != 0) {
			kprintf("vc_dispmanx_snapshot() failed\n");
			exit(EXIT_FAILURE);
		}

		VC_RECT_T rect;
		result = vc_dispmanx_rect_set(&rect, 0, 0, width, height);

		if (result != 0) {
			kprintf("vc_dispmanx_rect_set() failed\n");
			exit(EXIT_FAILURE);
		}

		result = vc_dispmanx_resource_read_data(resourceHandle, &rect, dmxImagePtr, scanline);

		if (result != 0) {
			kprintf("vc_dispmanx_resource_read_data() failed\n");
			exit(EXIT_FAILURE);
		}
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> timeout = end - start;

	std::cout << (timeout.count()) << std::endl;

	if (verbose) {
		kprintf("vc_dispmanx_resource_read_data() returned %d\n", result);
	}

	return 0;
}

ccolor *DispmanxScreenCapture::data() {
}
