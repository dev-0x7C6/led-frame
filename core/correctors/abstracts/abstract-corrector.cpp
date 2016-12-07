#include <core/correctors/abstracts/abstract-corrector.h>

using namespace Corrector::Abstract;

AbstractCorrector::AbstractCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner)
		, m_enabled(true)
		, m_priority(10)
		, m_factor(1.0) {
}

AbstractCorrector::AbstractCorrector(ci32 id, int owner, double factor)
		: AbstractCorrector(id, owner) {
	m_factor = factor;
}

AbstractCorrector::AbstractCorrector(ci32 id, int owner, double factor, u32 priority)
		: AbstractCorrector(id, owner, factor) {
	m_priority = priority;
}

bool AbstractCorrector::isEnabled() const { return m_enabled; }
double AbstractCorrector::factor() const { return m_factor; }
u32 AbstractCorrector::priority() const { return m_priority; }

void AbstractCorrector::push() {}
void AbstractCorrector::pop() {}

void AbstractCorrector::setEnabled(bool enabled) {
	if (m_enabled != enabled) {
		m_enabled = enabled;
		notify();
	}
}

void AbstractCorrector::setFactor(double factor) {
	m_factor = factor;
	notify();
}

void AbstractCorrector::setPriority(u32 priority) {
	if (m_priority != priority) {
		m_priority = priority;
		notify();
	}
}
