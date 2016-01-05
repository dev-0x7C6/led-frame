#include <core/containers/led-strip-info-container.h>

using namespace Container;

bool LedStripInfoContainer::clockwise() const {
	return m_clockwise;
}

void LedStripInfoContainer::setClockwise(bool clockwise) {
	m_clockwise = clockwise;
}

unsigned int LedStripInfoContainer::count() const {
	return m_count;
}

void LedStripInfoContainer::setCount(unsigned int count) {
	m_count = count;
}

Enum::ColorFormat LedStripInfoContainer::format() const {
	return m_format;
}

void LedStripInfoContainer::setFormat(const Enum::ColorFormat &format) {
	m_format = format;
}
