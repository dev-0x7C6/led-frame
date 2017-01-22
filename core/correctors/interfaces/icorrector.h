#pragma once

#include <core/enums/corrector-type.h>
#include <core/interfaces/inotify.h>
#include <core/types.h>
#include <atomic>

namespace Corrector {
namespace Interface {

class ICorrector : public ::Interface::INotify {
public:
	explicit ICorrector(ci32 id, int owner, factor_t factor = 1.0, u32 priority = 0, factor_t minFactor = 0, factor_t maxFactor = 1.0)
			: INotify(id)
			, m_owner(owner)
			, m_currentFactor(factor)
			, m_priority(priority)
			, m_minimumFactor(minFactor)
			, m_maximumFactor(maxFactor)

	{}
	virtual ~ICorrector() override = default;

	virtual Enum::CorrectorType type() const = 0;
	virtual color correct(ccolor value) const noexcept = 0;

	virtual void push() {}
	virtual void pop() {}

	auto owner() const noexcept { return m_owner; }

	auto factor() const noexcept -> factor_t { return m_currentFactor; }
	auto minimumFactor() const noexcept { return m_minimumFactor; }
	auto maximumFactor() const noexcept { return m_maximumFactor; }

	auto priority() const noexcept -> u32 { return m_priority; }

	auto isEnabled() const noexcept -> bool { return m_isEnabled; }
	auto isGlobal() const noexcept { return m_owner == -1; }

	inline auto setEnabled(bool value) noexcept;
	inline auto setFactor(factor_t value) noexcept;

private:
	const int m_owner = 0;
	std::atomic<factor_t> m_currentFactor{0};
	std::atomic<bool> m_isEnabled{false};
	u32 m_priority = 0;
	factor_t m_minimumFactor = 0;
	factor_t m_maximumFactor = 0;
};

auto ICorrector::setEnabled(bool value) noexcept {
	m_isEnabled = value;
	notify();
}

auto ICorrector::setFactor(factor_t value) noexcept {
	value = std::min(std::max(value, m_minimumFactor), m_maximumFactor);
	notify();
}
}
}
