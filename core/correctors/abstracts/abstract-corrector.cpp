#include <core/correctors/abstracts/abstract-corrector.h>

using namespace Corrector::Abstract;

AbstractCorrector::AbstractCorrector(const int owner)
		: Interface::ICorrector(owner)
		, m_enabled(true)
		, m_priority(10)
		, m_factor(1.0) {
}

AbstractCorrector::AbstractCorrector(const int owner, double factor)
		: AbstractCorrector(owner) {
	m_factor = factor;
}

AbstractCorrector::AbstractCorrector(const int owner, double factor, uint32_t priority)
		: AbstractCorrector(owner, factor) {
	m_priority = priority;
}

bool AbstractCorrector::isEnabled() const { return m_enabled; }
double AbstractCorrector::factor() const { return m_factor; }
uint32_t AbstractCorrector::priority() const { return m_priority; }

QJsonObject AbstractCorrector::parameters() const {
	return {
		{"corrector_id", id()},
		{"corrector_type", static_cast<const int>(type())},
		{"corrector_owner", owner()},
		{"corrector_factor_current", factor()},
		{"corrector_factor_min", maximumFactor()},
		{"corrector_factor_max", minimumFactor()},
	};
}

void AbstractCorrector::push() {}
void AbstractCorrector::pop() {}

void AbstractCorrector::setEnabled(bool enabled) {
	if (m_enabled != enabled) {
		m_enabled = enabled;
		notify();
	}
}
void AbstractCorrector::setFactor(double factor) {
	if (m_factor != factor) {
		m_factor = factor;
		notify();
	}
}
void AbstractCorrector::setPriority(uint32_t priority) {
	if (m_priority != priority) {
		m_priority = priority;
		notify();
	}
}
