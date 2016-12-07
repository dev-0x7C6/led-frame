#pragma once

#include "core/enums/position-enum.h"
#include "core/consts.h"

#include <array>

namespace Container {

constexpr auto SCANLINE_SIZE = 128u;
constexpr auto SCANLINE_LINE = 32u;

class ScanlineContainer final {
public:
	constexpr explicit ScanlineContainer() noexcept;
	constexpr explicit ScanlineContainer(const color fillColor) noexcept;

	constexpr static auto size() noexcept;
	constexpr static auto line() noexcept;

	inline auto data() noexcept;
	inline auto constData() const noexcept;

	inline auto data(const Enum::Position &position) noexcept;
	inline auto constData(const Enum::Position &position) const noexcept;

	inline void clear() noexcept;
	inline void fill(const color value) noexcept;
	inline void fill(const Enum::Position position, const color value) noexcept;

	inline void operator=(const ScanlineContainer &other) noexcept;
	inline void operator=(const color value) noexcept;
	inline bool operator==(const ScanlineContainer &other) const noexcept;
	inline bool operator!=(const ScanlineContainer &other) const noexcept;
	inline void operator<<(const color value) noexcept;
	inline void operator>>(const color value) noexcept;

public:
	constexpr static auto fromIndexToPosition(const std::size_t index) noexcept;
	static void interpolate(const ScanlineContainer &start, const ScanlineContainer &end, double progress, ScanlineContainer &out) noexcept;

private:
	std::array<color, SCANLINE_SIZE> m_data;
};

constexpr ScanlineContainer::ScanlineContainer() noexcept
		: m_data() {}
constexpr ScanlineContainer::ScanlineContainer(const color fillColor) noexcept
		: m_data() { m_data.fill(fillColor); }

constexpr auto ScanlineContainer::size() noexcept { return SCANLINE_SIZE; }
constexpr auto ScanlineContainer::line() noexcept { return SCANLINE_LINE; }

auto ScanlineContainer::data() noexcept { return m_data.data(); }
auto ScanlineContainer::constData() const noexcept { return m_data.data(); }

auto ScanlineContainer::data(const Enum::Position &position) noexcept { return m_data.data() + (static_cast<color>(position) * line()); }
auto ScanlineContainer::constData(const Enum::Position &position) const noexcept { return m_data.data() + (static_cast<color>(position) * line()); }

void ScanlineContainer::clear() noexcept { fill(0u); }
void ScanlineContainer::fill(const color value) noexcept { m_data.fill(value); }
void ScanlineContainer::fill(const Enum::Position position, const color value) noexcept {
	auto colors = data(position);

	for (auto i = 0u; i < size(); ++i)
		colors[i] = value;
}

constexpr auto ScanlineContainer::fromIndexToPosition(const std::size_t index) noexcept { return static_cast<Enum::Position>(index / ScanlineContainer::line()); }

void ScanlineContainer::operator=(const ScanlineContainer &other) noexcept { m_data = other.m_data; }
void ScanlineContainer::operator=(const color value) noexcept { fill(value); }
bool ScanlineContainer::operator==(const ScanlineContainer &other) const noexcept { return m_data == other.m_data; }
bool ScanlineContainer::operator!=(const ScanlineContainer &other) const noexcept { return !operator==(other); }

void ScanlineContainer::operator<<(const color value) noexcept {
	std::rotate(m_data.begin(), m_data.begin() + 1, m_data.end());
	m_data[SCANLINE_SIZE - 1] = value;
}

void ScanlineContainer::operator>>(const color value) noexcept {
	std::rotate(m_data.rbegin(), m_data.rbegin() + 1, m_data.rend());
	m_data[0] = value;
}

static_assert(sizeof(ScanlineContainer) == ScanlineContainer::size() * sizeof(color), "ScanlineContainer should fit in ScanlineSize");
static_assert(alignof(ScanlineContainer) == sizeof(color), "ScanlineContainer should be align to sizeof(color)");
}
