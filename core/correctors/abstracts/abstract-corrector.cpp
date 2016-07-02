#include <core/correctors/abstracts/abstract-corrector.h>

using namespace Corrector::Abstract;

AbstractCorrector::AbstractCorrector()
		: m_enabled(true)
		, m_priority(10)
		, m_factor(1.0) {}

AbstractCorrector::AbstractCorrector(float factor)
		: AbstractCorrector() {
	m_factor = factor;
}

AbstractCorrector::AbstractCorrector(float factor, uint32_t priority)
		: AbstractCorrector(factor) {
	m_priority = priority;
}

bool AbstractCorrector::enabled() const { return m_enabled; }
float AbstractCorrector::factor() const { return m_factor; }
uint32_t AbstractCorrector::priority() const { return m_priority; }

void AbstractCorrector::setEnabled(bool enabled) {
	if (m_enabled != enabled) {
		m_enabled = enabled;
		notify();
	}
}
void AbstractCorrector::setFactor(float factor) {
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
