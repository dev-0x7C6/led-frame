#include "dispmanx-helper.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::literals;
using namespace Functional::Helper;

DispmanxHelper::DispmanxHelper() {
	open();
	queryMode(m_mode);
	allocate(m_mode);
	createResource(m_mode);
}

DispmanxHelper::~DispmanxHelper() {
	deallocate();
	freeResource();
	close();
}

bool DispmanxHelper::open() noexcept {
	m_display = vc_dispmanx_display_open(0);
	if (m_display == 0) {
		std::cerr << "rpi: opening display failed!" << std::endl;
		std::this_thread::sleep_for(1s);
		return false;
	}

	return true;
}

bool DispmanxHelper::query(DISPMANX_MODEINFO_T &mode) noexcept {
	if (m_display == 0) {
		std::cerr << "rpi: display should be open, but is not!" << std::end;
		std::this_thread::sleep_for(1s);
		return false;
	}

	if (vc_dispmanx_display_get_info(m_display, &mode)) noexcept {
			std::cerr << "rpi: unable to get display informations." << std::endl;
			std::this_thread::sleep_for(1s);
			return false;
		}

	return true;
}

bool DispmanxHelper::allocate(const DISPMANX_MODEINFO_T &mode) noexcept {
	const auto size = mode.width * mode.height * 4;

	if (m_data == nullptr)
		m_data = std::malloc(size);

	if (size != m_allocated)
		m_data = std::realloc(m_data, size);

	m_allocated = size;

	if (m_data == nullptr) {
		std::cerr << "rpi: memory allocation failed!" << std::endl;
		std::this_thread::sleep_for(1s);
		return false;
	}

	m_w = mode.width;
	m_h = mode.height;

	return true;
}

bool DispmanxHelper::createResource(const DISPMANX_MODEINFO_T &mode) noexcept {
	if (m_resource != 0)
		return true;

	const VC_IMAGE_TYPE_T type = VC_IMAGE_RGBA32;
	uint32_t vcImagePtr = 0;
	auto id = vc_dispmanx_resource_create(type, mode.width, mode.height, &vcImagePtr);

	if (id == 0) {
		std::cerr << "rpi: dispmanx resource failed!" << std::endl;
		std::this_thread::sleep_for(1s);
		return false;
	}

	m_resource = id;
	return true;
}

void DispmanxHelper::deallocate() noexcept {
	if (m_data != nullptr)
		std::free(m_data);
}

bool DispmanxHelper::freeResource() noexcept {
	if (m_resource != 0)
		vc_dispmanx_resource_delete(m_resource);
}

void DispmanxHelper::close() noexcept {
	if (m_display != 0)
		vc_dispmanx_display_close(m_display);
}

bool DispmanxHelper::capture() {
	if (m_display == 0) {
		std::cerr << "rpi: display should be open, but is not!" << std::endl;
		std::this_thread::sleep_for(1s);
		std::cerr << "rpi: trying to reopen!" << std::endl;
		open();
		return false;
	}

	DISPMANX_MODEINFO_T mode;
	query(mode);
	allocate(mode);
	createResource(mode);

	auto restartDispmanx = [this]() {
		std::cerr << "rpi: freeing resources." << std::endl;
		freeResource();
		std::cerr << "rpi: closing display." << std::endl;
		close();
		std::cerr << "rpi: reopening display." << std::endl;
		open();
	};

	if (vc_dispmanx_snapshot(m_display, m_resource, DISPMANX_NO_ROTATE)) {
		std::cerr << "vc_dispmanx_snapshot failed!" << std::endl;
		std::this_thread::sleep_for(1s);
		restartDispmanx();
		return false;
	}

	VC_RECT_T rect;
	if (vc_dispmanx_rect_set(&rect, 0, 0, m_mode.width, m_mode.height)) {
		std::cerr << "vc_dispmanx_rect_set failed!" << std::endl;
		std::this_thread::sleep_for(1s);
		restartDispmanx();
		return false;
	}

	if (vc_dispmanx_resource_read_data(m_resource, &rect, m_data, m_mode.width * 4)) {
		std::cerr << "vc_dispmanx_resource_read_data failed!" << std::endl;
		std::this_thread::sleep_for(1s);
		restartDispmanx();
		return false;
	}

	return true;
}

int DispmanxHelper::width() const noexcept { return m_w; }
int DispmanxHelper::height() const noexcept { return m_h; }

DispmanxHelper::data() const noexcept { return m_data; }
