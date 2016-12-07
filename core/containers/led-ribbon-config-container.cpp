#include <core/containers/led-ribbon-config-container.h>

#include <type_traits>

using namespace Enum;
using namespace Container;

LedRibbonConfigContainer::LedRibbonConfigContainer(const Struct::LedRibbonConfigStruct &config)
		: m_config(config) {
}

ContainerType LedRibbonConfigContainer::type() const {
	return ContainerType::LedRibbonConfig;
}

void LedRibbonConfigContainer::setColorFormat(const ColorFormat &format) {
	m_config.format = static_cast<std::underlying_type<ColorFormat>::type>(format);
}

void LedRibbonConfigContainer::setCount(const u8 &count) {
	m_config.count = count;
}

void LedRibbonConfigContainer::setDirection(const Direction &direction) {
	m_config.direction = static_cast<std::underlying_type<Direction>::type>(direction);
}

void LedRibbonConfigContainer::setPosition(const Position &position) {
	m_config.position = static_cast<std::underlying_type<Position>::type>(position);
}

const Struct::LedRibbonConfigStruct &LedRibbonConfigContainer::config() const {
	return m_config;
}
