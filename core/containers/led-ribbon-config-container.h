#pragma once

#include <core/enums/direction.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <core/types.h>

namespace Container {

struct RibbonBitField {
	u8 count;
	u8 direction;
	u8 format;
	u8 position;

	void operator<<(u8 *&data) noexcept {
		const auto value = data[0] | data[1] << 8u;
		// uucc | cccc | ccff | fppd
		direction = (value >> 0u) & 0b0001;
		position = (value >> 1u) & 0b0011;
		format = (value >> 3u) & 0b111;
		count = (value >> 6u) & 0xff;
		data += 2;
	}

	void operator>>(u8 *&data) {
		auto *wdata = reinterpret_cast<u16 *>(data);
		wdata[0] =
			(static_cast<u16>(direction) << 0u) |
			(static_cast<u16>(position) << 1u) |
			(static_cast<u16>(format) << 3u) |
			(static_cast<u16>(count) << 6u);
		data += 2;
	}
};

static_assert(sizeof(RibbonBitField) == 4, "");

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
constexpr auto RibbonConfiguration::colorFormat() const noexcept -> Enum::ColorFormat { return static_cast<Enum::ColorFormat>(m_data.format); }
constexpr auto RibbonConfiguration::direction() const noexcept -> Enum::Direction { return static_cast<Enum::Direction>(m_data.direction); }
constexpr auto RibbonConfiguration::position() const noexcept -> Enum::Position { return static_cast<Enum::Position>(m_data.position); }
constexpr auto RibbonConfiguration::count() const noexcept -> u8 { return static_cast<u8>(m_data.count); }
}
