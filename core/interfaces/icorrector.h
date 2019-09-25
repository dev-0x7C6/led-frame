#pragma once

#include <core/containers/color-scanline-container.h>
#include <core/enums/corrector-type.h>
#include <core/enums/priority-enum.h>
#include <core/types.h>

#include <atomic>

#include <core/generic/irepresentable.h>

#include <QSettings>

class ICorrector : public IRepresentable {
public:
	inline explicit ICorrector(int owner, Enum::Priority priority = Enum::Priority::Average);

	auto category() const noexcept -> Category final { return Category::Corrector; }
	auto properties() const noexcept -> Properties final {
		return {
			{"id", id()},
			{"type", value(type())},
			{"owner", m_owner},
			{"factor", static_cast<int>(m_factor.value())},
			{"min", static_cast<int>(m_factor.min())},
			{"max", static_cast<int>(m_factor.max())},
		};
	}

	virtual CorrectorType type() const noexcept = 0;
	virtual void correct(Container::Scanline &scanline) const noexcept = 0;

	auto owner() const noexcept { return m_owner; }

	const auto &factor() const noexcept { return m_factor; }
	const auto &threshold() const noexcept { return m_threshold; }

	auto priority() const noexcept { return m_priority; }

	auto isEnabled() const noexcept -> bool { return m_enabled == 1; }
	auto isGlobal() const noexcept { return m_owner == -1; }

	inline auto setEnabled(bool value) noexcept;
	inline auto setFactor(correct_t value) noexcept;

	void save(QSettings &settings) {
		settings.setValue("factor", m_factor.value());
		settings.setValue("threshold", m_threshold.value());
		settings.setValue("enabled", static_cast<int>(m_enabled));
	}

	void load(QSettings &settings) {
		m_factor.setValue(settings.value("factor", m_factor.value()).toUInt());
		m_threshold.setValue(settings.value("threshold", m_threshold.value()).toUInt());
		m_enabled = settings.value("enabled", static_cast<int>(m_enabled)).toInt();
	}

protected:
	FactorModifier m_factor{255, 255, 0};
	FactorModifier m_threshold{255, 255, 0};
	std::atomic<int> m_enabled{1};

private:
	const int m_owner = 0;
	Enum::Priority m_priority = Enum::Priority::Average;
};

// impl

ICorrector::ICorrector(int owner, const Enum::Priority priority)
		: m_owner(owner)
		, m_priority(priority)

{}

auto ICorrector::setEnabled(bool value) noexcept {
	m_enabled = value ? 1 : 0;
	notify();
}

auto ICorrector::setFactor(correct_t value) noexcept {
	m_factor.setValue(std::min(std::max(value, m_factor.min()), m_factor.max()));
	notify();
}
