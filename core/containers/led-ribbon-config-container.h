#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/direction.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <cstdint>

namespace Container {

namespace Struct {
struct LedRibbonConfigStruct {
	uint16_t direction : 1;
	uint16_t position : 2;
	uint16_t format : 3;
	uint16_t count : 8;
} __attribute__((packed));

static_assert(sizeof(LedRibbonConfigStruct) == 2, "Struct size is different than expected.");
}

class LedRibbonConfigContainer final : public AbstractContainer {
public:
	explicit LedRibbonConfigContainer() = default;
	explicit LedRibbonConfigContainer(const Struct::LedRibbonConfigStruct &config);
	virtual ~LedRibbonConfigContainer() = default;

	virtual Enum::ContainerType type() const override;

	inline Enum::ColorFormat colorFormat() const;
	inline Enum::Direction direction() const;
	inline Enum::Position position() const;
	inline uint8_t count() const;

	void setColorFormat(const Enum::ColorFormat &format);
	void setCount(const uint8_t &count);
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

uint8_t LedRibbonConfigContainer::count() const {
	return static_cast<uint8_t>(m_config.count);
}
}
