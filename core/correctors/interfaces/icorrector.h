#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/corrector-type.h>
#include <core/enums/priority-enum.h>
#include <core/interfaces/inotify.h>
#include <core/types.h>

#include <atomic>

namespace Corrector {
namespace Interface {

class ICorrector : public ::Interface::INotify {
public:
	inline explicit ICorrector(ci32 id, int owner, const Enum::Priority priority = Enum::Priority::Average);
	virtual ~ICorrector() override = default;

	virtual Enum::CorrectorType type() const = 0;
	virtual void correct(Container::Scanline &scanline) const noexcept = 0;

	auto owner() const noexcept { return m_owner; }

	const auto &factor() const noexcept { return m_factor; }
	const auto &threshold() const noexcept { return m_threshold; }

	auto priority() const noexcept { return m_priority; }

	auto isEnabled() const noexcept -> bool { return m_enabled; }
	auto isGlobal() const noexcept { return m_owner == -1; }

	inline auto setEnabled(bool value) noexcept;
	inline auto setFactor(correct_t value) noexcept;

protected:
	FactorModifier m_factor{255, 255, 0};
	FactorModifier m_threshold{255, 255, 0};
	std::atomic<bool> m_enabled{true};

private:
	const int m_owner = 0;
	Enum::Priority m_priority = Enum::Priority::Average;
};

// impl

ICorrector::ICorrector(ci32 id, int owner, const Enum::Priority priority)
		: INotify(id)
		, m_owner(owner)
		, m_priority(priority)

{}

auto ICorrector::setEnabled(bool value) noexcept {
	m_enabled = value;
	notify();
}

auto ICorrector::setFactor(correct_t value) noexcept {
	m_factor.setValue(std::min(std::max(value, m_factor.min()), m_factor.max()));
	notify();
}
}
}
