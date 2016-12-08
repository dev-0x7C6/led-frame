#include <core/containers/led-ribbon-config-container.h>

#include <type_traits>

using namespace Enum;
using namespace Container;

void RibbonConfiguration::setColorFormat(const ColorFormat format) noexcept {
	m_data.format = static_cast<u16>(format);
}

void RibbonConfiguration::setCount(cu8 count) noexcept {
	m_data.count = count;
}

void RibbonConfiguration::setDirection(const Direction direction) noexcept {
	m_data.direction = static_cast<u16>(direction);
}

void RibbonConfiguration::setPosition(const Position position) noexcept {
	m_data.position = static_cast<u16>(position);
}

void RibbonConfiguration::operator=(const RibbonConfiguration &other) noexcept {
	m_data = other.m_data;
}
