#pragma once

#include <core/enums/direction.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include "core/types.h"

namespace Container {

union RibbonBitField {
	u16 raw;

	struct {
		u16 direction : 1;
		u16 position : 2;
		u16 format : 3;
		u16 count : 8;
		u16 unused : 2;
	} bitfield;
};

class RibbonConfiguration final {
public:
	constexpr explicit RibbonConfiguration() noexcept;
	constexpr explicit RibbonConfiguration(const RibbonBitField field) noexcept;

	constexpr auto colorFormat() const noexcept -> Enum::ColorFormat;
	constexpr auto count() const noexcept -> u8;
	constexpr auto direction() const noexcept -> Enum::Direction;
	constexpr auto position() const noexcept -> Enum::Position;

	void setColorFormat(const Enum::ColorFormat format) noexcept;
	void setCount(cu8 count) noexcept;
	void setDirection(const Enum::Direction direction) noexcept;
	void setPosition(const Enum::Position position) noexcept;

	void operator=(const RibbonConfiguration &other) noexcept;

	constexpr auto rawData() const noexcept { return m_data; }

private:
	RibbonBitField m_data;
};

constexpr RibbonConfiguration::RibbonConfiguration() noexcept
		: m_data() {}
constexpr RibbonConfiguration::RibbonConfiguration(const RibbonBitField field) noexcept
		: m_data(field) {}
constexpr auto RibbonConfiguration::colorFormat() const noexcept -> Enum::ColorFormat { return static_cast<Enum::ColorFormat>(m_data.bitfield.format); }
constexpr auto RibbonConfiguration::direction() const noexcept -> Enum::Direction { return static_cast<Enum::Direction>(m_data.bitfield.direction); }
constexpr auto RibbonConfiguration::position() const noexcept -> Enum::Position { return static_cast<Enum::Position>(m_data.bitfield.position); }
constexpr auto RibbonConfiguration::count() const noexcept -> u8 { return static_cast<u8>(m_data.bitfield.count); }

static_assert(std::is_pod<RibbonBitField>::value, "should be POD");
static_assert(alignof(RibbonBitField) == sizeof(u16), "align should be same as u16.");
static_assert(sizeof(RibbonBitField) == sizeof(u16), "size is different than expected.");
static_assert(sizeof(RibbonConfiguration) == sizeof(u16), "size is different than expected.");
}
