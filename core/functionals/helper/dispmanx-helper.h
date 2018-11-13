#pragma once

#include "bcm_host.h"
#include "interface/vmcs_host/vcgencmd.h"

#include <atomic>

namespace Functional {
namespace Helper {

class DispmanxHelper final {
public:
	explicit DispmanxHelper();
	virtual ~DispmanxHelper();

	bool capture();

	auto width() const noexcept -> int;
	auto height() const noexcept -> int;
	auto data() const noexcept -> void *;

protected:
	bool open() noexcept;
	bool query(DISPMANX_MODEINFO_T &mode) noexcept;
	bool allocate(const DISPMANX_MODEINFO_T &mode) noexcept;
	void deallocate() noexcept;
	bool createResource(const DISPMANX_MODEINFO_T &mode) noexcept;
	void freeResource() noexcept;
	void close() noexcept;

private:
	DISPMANX_DISPLAY_HANDLE_T m_display = 0;
	DISPMANX_RESOURCE_HANDLE_T m_resource = 0;
	void *m_data = nullptr;
	int m_allocated = 0;
	std::atomic<int> m_w{0};
	std::atomic<int> m_h{0};
};
} // namespace Helper
} // namespace Functional
