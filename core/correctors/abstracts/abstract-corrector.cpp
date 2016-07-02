#include <core/correctors/abstracts/abstract-corrector.h>

using namespace Corrector::Abstract;

static std::atomic<int> internalCorrectorId{0};

AbstractCorrector::AbstractCorrector()
		: m_id(internalCorrectorId++)
		, m_enabled(true)
		, m_priority(10)
		, m_factor(1.0) {
}

int AbstractCorrector::id() const { return m_id; }

AbstractCorrector::AbstractCorrector(double factor)
		: AbstractCorrector() {
	m_factor = factor;
}

AbstractCorrector::AbstractCorrector(double factor, uint32_t priority)
		: AbstractCorrector(factor) {
	m_priority = priority;
}

bool AbstractCorrector::enabled() const { return m_enabled; }
double AbstractCorrector::factor() const { return m_factor; }
uint32_t AbstractCorrector::priority() const { return m_priority; }

QJsonObject AbstractCorrector::parameters() const {
	return {
		{"id", static_cast<int>(m_id)},
		{"type", static_cast<const int>(type())},
		{"factor", factor()},
		{"maximumFactor", maximumFactor()},
		{"minimumFactor", minimumFactor()},
	};
}

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
