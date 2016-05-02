#include <core/abstracts/abstract-corrector.h>

using namespace Abstract;

AbstractCorrector::AbstractCorrector()
	: m_enabled(true)
	, m_priority(10)
	, m_factor(1.0)

{
}

AbstractCorrector::AbstractCorrector(const float &factor)
	: AbstractCorrector()

{
	m_factor = factor;
}

bool AbstractCorrector::enabled() const {
	return m_enabled;
}

float AbstractCorrector::factor() const {
	return m_factor;
}

uint32_t AbstractCorrector::priority() const {
	return m_priority;
}

void AbstractCorrector::setEnabled(const bool &enabled) {
	m_enabled = enabled;
}

void AbstractCorrector::setFactor(const float &factor) {
	m_factor = factor;
}

void AbstractCorrector::setPriority(const uint32_t &priority) {
	m_priority = priority;
}
