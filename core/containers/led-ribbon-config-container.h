#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/direction.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include "core/consts.h"

namespace Container {

namespace Struct {
struct LedRibbonConfigStruct {
	u16 direction : 1;
	u16 position : 2;
	u16 format : 3;
	u16 count : 8;
} __attribute__((packed));

static_assert(sizeof(LedRibbonConfigStruct) == 2, "Struct size is different than expected.");
}

class LedRibbonConfigContainer final : public AbstractContainer {
public:
	explicit LedRibbonConfigContainer() = default;
	explicit LedRibbonConfigContainer(const Struct::LedRibbonConfigStruct &config);
	~LedRibbonConfigContainer() override = default;

	Enum::ContainerType type() const override;

	inline Enum::ColorFormat colorFormat() const;
	inline Enum::Direction direction() const;
	inline Enum::Position position() const;
	inline u8 count() const;

	void setColorFormat(const Enum::ColorFormat &format);
	void setCount(const u8 &count);
	void setDirection(const Enum::Direction &direction);
	void setPosition(const Enum::Position &position);

	const Struct::LedRibbonConfigStruct &config() const;

private:
	Struct::LedRibbonConfigStruct m_config;
};

Enum::ColorFormat LedRibbonConfigContainer::colorFormat() const {
	return static_cast<Enum::ColorFormat>(m_config.format);
}

Enum::Direction LedRibbonConfigContainer::direction() const {
	return static_cast<Enum::Direction>(m_config.direction);
}

Enum::Position LedRibbonConfigContainer::position() const {
	return static_cast<Enum::Position>(m_config.position);
}

u8 LedRibbonConfigContainer::count() const {
	return static_cast<u8>(m_config.count);
}
}
