#include <core/abstracts/abstract-corrector-attach.h>
#include <algorithm>

using namespace Abstract;

void AbstractCorrectorAttach::attach(const std::shared_ptr<Interface::IColorCorrector> &corrector) {
	m_correctors.emplace_back(corrector);
	std::sort(m_correctors.begin(), m_correctors.end(), [](const auto & a, auto & b) {
		return (a->priority() > b->priority());
	});
}

uint32_t AbstractCorrectorAttach::execute(const uint32_t &color) {
	uint32_t base = color;

	for (const auto &corrector : m_correctors)
		base = corrector->correct(base);

	return base;
}