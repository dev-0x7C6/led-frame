#pragma once

#include <atomic>

namespace Functional {

using ReferenceCounter = std::atomic<int>;

class RaiiReferenceCounter final {
public:
	explicit RaiiReferenceCounter(ReferenceCounter &counter) noexcept
			: m_counter(counter) { ++m_counter; }
	~RaiiReferenceCounter() noexcept { --m_counter; }

private:
	ReferenceCounter &m_counter;
};
} // namespace Functional
