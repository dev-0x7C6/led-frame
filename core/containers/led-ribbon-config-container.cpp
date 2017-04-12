#include <core/containers/led-ribbon-config-container.h>

#include <type_traits>

using namespace Enum;
using namespace Container;

void RibbonConfiguration::setColorFormat(const ColorFormat format) noexcept {
	m_data.setFormat(static_cast<u8>(format));
}

void RibbonConfiguration::setCount(cu8 count) noexcept {
	m_data.setFormat(static_cast<u8>(count));
}

void RibbonConfiguration::setDirection(const Direction direction) noexcept {
	m_data.setDirection(static_cast<u8>(direction));
}

void RibbonConfiguration::setPosition(const Position position) noexcept {
	m_data.setPosition(static_cast<u8>(position));
}

void RibbonConfiguration::operator=(const RibbonConfiguration &other) noexcept {
	m_data = other.m_data;
}
