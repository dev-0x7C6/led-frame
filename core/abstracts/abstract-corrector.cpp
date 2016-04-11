#include <core/abstracts/abstract-corrector.h>

using namespace Abstract;

bool AbstractCorrector::enabled() const {
	return m_enabled;
}

uint32_t AbstractCorrector::priority() const {
	return m_priority;
}

void AbstractCorrector::setEnabled(const bool &enabled) {
	m_enabled = enabled;
}

void AbstractCorrector::setPriority(const uint32_t &priority) {
	m_priority = priority;
}
