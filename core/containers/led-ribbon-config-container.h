#pragma once

#include <core/enums/direction.h>
#include <core/enums/color-format-enum.h>
#include <core/enums/position-enum.h>

#include <core/types.h>

namespace Container {

class RibbonBitField {
public:
	constexpr explicit RibbonBitField() = default;

	void fromData(cu16 data) noexcept {
		m_direction = get_direction(data);
		m_position = get_position(data);
		m_format = get_format(data);
		m_count = get_count(data);
	}

	void operator<<(u8 *&data) noexcept {
		fromData(static_cast<u16>(data[0] | data[1] << 8u));
		data += 2;
	}

	void operator>>(u8 *&data) {
		auto *wdata = reinterpret_cast<u16 *>(data);
		wdata[0] =
			put_direction(m_direction) |
			put_position(m_position) |
			put_format(m_format) |
			put_count(m_count);
		data += 2;
	}

	constexpr auto count() const noexcept { return m_count; }
	constexpr auto direction() const noexcept { return m_direction; }
	constexpr auto format() const noexcept { return m_format; }
	constexpr auto position() const noexcept { return m_position; }

	void setCount(cu8 count) noexcept { m_count = count; }
	void setDirection(cu8 direction) noexcept { m_direction = direction; }
	void setFormat(cu8 format) noexcept { m_format = format; }
	void setPosition(cu8 position) noexcept { m_position = position; }

private:
	u8 m_count{};
	u8 m_direction{};
	u8 m_format{};
	u8 m_position{};

private:
	constexpr static auto direction_shift = 0;
	constexpr static auto position_shift = 1;
	constexpr static auto format_shift = 3;
	constexpr static auto count_shift = 6;

	constexpr static auto direction_mask = 0b00000001;
	constexpr static auto position_mask = 0b00000011;
	constexpr static auto format_mask = 0b00000111;
	constexpr static auto count_mask = 0b11111111;

	constexpr static u8 get_direction(cu16 value) { return (value >> direction_shift) & direction_mask; }
	constexpr static u8 get_position(cu16 value) { return (value >> position_shift) & position_mask; }
	constexpr static u8 get_format(cu16 value) { return (value >> format_shift) & format_mask; }
	constexpr static u8 get_count(cu16 value) { return (value >> count_shift) & count_mask; }

	constexpr static u16 put_direction(cu8 value) { return static_cast<u16>(value << direction_shift); }
	constexpr static u16 put_position(cu8 value) { return static_cast<u16>(value << position_shift); }
	constexpr static u16 put_format(cu8 value) { return static_cast<u16>(value << format_shift); }
	constexpr static u16 put_count(cu8 value) { return static_cast<u16>(value << count_shift); }
};

static_assert(sizeof(RibbonBitField) == 4);

class RibbonConfiguration final {
public:
	constexpr explicit RibbonConfiguration() noexcept;
	constexpr explicit RibbonConfiguration(RibbonBitField field) noexcept;

	constexpr auto colorFormat() const noexcept -> Enum::ColorFormat;
	constexpr auto count() const noexcept -> u8;
	constexpr auto direction() const noexcept -> Enum::Direction;
	constexpr auto position() const noexcept -> Enum::Position;

	void setColorFormat(Enum::ColorFormat format) noexcept;
	void setCount(cu8 count) noexcept;
	void setDirection(Enum::Direction direction) noexcept;
	void setPosition(Enum::Position position) noexcept;

	constexpr auto rawData() const noexcept { return m_data; }

private:
	RibbonBitField m_data;
};

constexpr RibbonConfiguration::RibbonConfiguration() noexcept
		: m_data() {}
constexpr RibbonConfiguration::RibbonConfiguration(const RibbonBitField field) noexcept
		: m_data(field) {}
constexpr auto RibbonConfiguration::colorFormat() const noexcept -> Enum::ColorFormat { return static_cast<Enum::ColorFormat>(m_data.format()); }
constexpr auto RibbonConfiguration::direction() const noexcept -> Enum::Direction { return static_cast<Enum::Direction>(m_data.direction()); }
constexpr auto RibbonConfiguration::position() const noexcept -> Enum::Position { return static_cast<Enum::Position>(m_data.position()); }
constexpr auto RibbonConfiguration::count() const noexcept -> u8 { return static_cast<u8>(m_data.count()); }
} // namespace Container
