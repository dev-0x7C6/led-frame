#pragma once

#include <atomic>
#include <cstdint>
#include <limits>
#include <type_traits>

#include <externals/common/types.hpp>

template <typename type>
constexpr auto is_class_efficient_func() noexcept {
	return std::is_copy_assignable_v<type> &&
		std::is_copy_constructible_v<type> &&
		std::is_move_assignable_v<type> &&
		std::is_move_constructible_v<type>;
}

template <typename type>
constexpr auto is_class_efficient_nothrow_func() noexcept {
	return std::is_nothrow_copy_assignable_v<type> &&
		std::is_nothrow_copy_constructible_v<type> &&
		std::is_nothrow_move_assignable_v<type> &&
		std::is_nothrow_move_constructible_v<type>;
}

template <typename type>
class is_class_cxx14_efficient : public std::integral_constant<bool, is_class_efficient_func<type>()> {};

template <typename type>
class is_class_cxx14_efficient_nothrow : public std::integral_constant<bool, is_class_efficient_nothrow_func<type>()> {};

static_assert(std::is_same_v<i16, int16_t>);
static_assert(std::is_same_v<i32, int32_t>);
static_assert(std::is_same_v<i64, int64_t>);
static_assert(std::is_same_v<i8, int8_t>);
static_assert(std::is_same_v<u16, uint16_t>);
static_assert(std::is_same_v<u32, uint32_t>);
static_assert(std::is_same_v<u64, uint64_t>);
static_assert(std::is_same_v<u8, uint8_t>);

static_assert(sizeof(i16) == 2);
static_assert(sizeof(i32) == 4);
static_assert(sizeof(i64) == 8);
static_assert(sizeof(i8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);
static_assert(sizeof(u8) == 1);

static_assert(std::is_signed_v<i16>);
static_assert(std::is_signed_v<i32>);
static_assert(std::is_signed_v<i64>);
static_assert(std::is_signed_v<i8>);
static_assert(std::is_unsigned_v<u16>);
static_assert(std::is_unsigned_v<u32>);
static_assert(std::is_unsigned_v<u64>);
static_assert(std::is_unsigned_v<u8>);

using factor_t = float;
using cfactor = const factor_t;

using color = u32;

using cchar = const char;
using correct_t = u32;

template <typename type>
class FactorModifierTempate {
public:
	constexpr FactorModifierTempate(type value, type max, type min) noexcept
			: m_value(value)
			, m_max(max)
			, m_min(min) {}

	constexpr FactorModifierTempate(const FactorModifierTempate &rhs) = default;
	constexpr FactorModifierTempate &operator=(const FactorModifierTempate &rhs) = default;

	FactorModifierTempate &operator=(FactorModifierTempate &&rhs) noexcept {
		m_max = std::move(rhs.m_max);
		m_min = std::move(rhs.m_min);
		m_value = value();
		return *this;
	}

	FactorModifierTempate(FactorModifierTempate &&rhs) noexcept
			: m_value(rhs.value())
			, m_max(std::move(rhs.m_max))
			, m_min(std::move(rhs.m_min)) {
	}

	~FactorModifierTempate() = default;

	constexpr auto value() const noexcept -> type { return m_value; }
	constexpr auto max() const noexcept { return m_max; }
	constexpr auto min() const noexcept { return m_min; }

	constexpr auto setValue(const type value) { m_value = value; }

	constexpr auto factor() const noexcept {
		const auto value = static_cast<factor_t>(m_value - m_min);
		const auto max = static_cast<factor_t>(m_max - m_min);
		return value / max;
	}

	constexpr auto isMinimum() const noexcept {
		return m_value == m_min;
	}

	constexpr auto isMaximum() const noexcept {
		return m_value == m_max;
	}

private:
	std::atomic<type> m_value{std::numeric_limits<type>::max()};
	type m_max = std::numeric_limits<type>::max();
	type m_min = std::numeric_limits<type>::min();
};

using FactorModifier = FactorModifierTempate<correct_t>;
