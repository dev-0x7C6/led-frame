#pragma once

#include <chrono>
#include <core/types.h>

namespace Functional {

class FramePaceSync {
public:
	explicit FramePaceSync(u64 hz);

	auto hz() const noexcept { return m_hz; }
	factor_t timing() const noexcept { return (static_cast<factor_t>(1.0) / m_hz); }

	void synchronize();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_point;
	std::chrono::nanoseconds m_perfectFrameTiming;
	const u64 m_hz{100};
};
} // namespace Functional
