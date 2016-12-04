#pragma once

#include <core/containers/abstract-container.h>
#include <core/enums/position-enum.h>

#include <array>
#include <cstdint>

namespace Container {

constexpr auto SCANLINE_SIZE = 128u;
constexpr auto SCANLINE_LINE = 32u;

using color = uint32_t;

class ScanlineContainer final {
public:
	constexpr explicit ScanlineContainer()
			: m_data() {}
	constexpr explicit ScanlineContainer(const color fillColor)
			: m_data() { m_data.fill(fillColor); }

	constexpr static auto size() noexcept { return SCANLINE_SIZE; }
	constexpr static auto line() noexcept { return SCANLINE_LINE; }

	inline auto data() noexcept { return m_data.data(); }
	inline auto data(const Enum::Position &position) noexcept { return m_data.data() + (static_cast<color>(position) * line()); }
	inline auto constData() const noexcept { return m_data.data(); }
	inline auto constData(const Enum::Position &position) const noexcept { return m_data.data() + (static_cast<color>(position) * line()); }

	inline void fill(const color value) noexcept { m_data.fill(value); }
	inline void fill(const Enum::Position position, const color value) noexcept {
		auto colors = data(position);

		for (auto i = 0u; i < size(); ++i)
			colors[i] = value;
	}

	inline void clear() noexcept { fill(0u); }

	constexpr static auto fromIndexToPosition(const std::size_t index) noexcept { return static_cast<Enum::Position>(index / ScanlineContainer::line()); }
	static void interpolate(const ScanlineContainer &start, const ScanlineContainer &end, double progress, ScanlineContainer &out) noexcept;

	inline void operator=(const ScanlineContainer &other) noexcept { m_data = other.m_data; }
	inline void operator=(const color value) noexcept { fill(value); }

	inline void operator<<(const color value) noexcept {
		std::rotate(m_data.begin(), m_data.begin() + 1, m_data.end());
		m_data[SCANLINE_SIZE - 1] = value;
	}

	inline void operator>>(const color value) noexcept {
		std::rotate(m_data.rbegin(), m_data.rbegin() + 1, m_data.rend());
		m_data[0] = value;
	}

	inline bool operator==(const ScanlineContainer &other) const noexcept { return m_data == other.m_data; }
	inline bool operator!=(const ScanlineContainer &other) const noexcept { return !operator==(other); }

private:
	std::array<uint32_t, SCANLINE_SIZE> m_data;
};

static_assert(sizeof(ScanlineContainer) == ScanlineContainer::size() * sizeof(color), "ScanlineContainer should fit in ScanlineSize");
static_assert(alignof(ScanlineContainer) == sizeof(color), "ScanlineContainer should be align to sizeof(color)");
}
