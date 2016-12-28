#pragma once

#include <core/enums/position-enum.h>
#include <core/functionals/color-functions.h>
#include <core/types.h>

#include <algorithm>
#include <array>

namespace Container {

template <int linesize>
class ScanlineContainer final {
public:
	static_assert(linesize % 4 == 0, "modulo by 4 should be 0.");

	inline explicit ScanlineContainer() noexcept;
	inline explicit ScanlineContainer(const color fillColor) noexcept;

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

	static color interpolation(ccolor start, ccolor end, double p);
	static void interpolate(const ScanlineContainer &start, const ScanlineContainer &end, double progress, ScanlineContainer &out) noexcept;

	template <int newsize>
	inline ScanlineContainer<newsize> resize();

private:
	std::array<color, linesize> m_data;
};

using Scanline = ScanlineContainer<128u>;

template <int linesize>
inline ScanlineContainer<linesize>::ScanlineContainer() noexcept
		: m_data() {}

template <int linesize>
inline ScanlineContainer<linesize>::ScanlineContainer(const color fillColor) noexcept
		: m_data() { m_data.fill(fillColor); }

template <int linesize>
constexpr auto ScanlineContainer<linesize>::size() noexcept { return linesize; }

template <int linesize>
constexpr auto ScanlineContainer<linesize>::line() noexcept { return linesize / 4; }

template <int linesize>
auto ScanlineContainer<linesize>::data() noexcept { return m_data.data(); }

template <int linesize>
auto ScanlineContainer<linesize>::constData() const noexcept { return m_data.data(); }

template <int linesize>
auto ScanlineContainer<linesize>::data(const Enum::Position &position) noexcept { return m_data.data() + (static_cast<color>(position) * line()); }

template <int linesize>
auto ScanlineContainer<linesize>::constData(const Enum::Position &position) const noexcept { return m_data.data() + (static_cast<color>(position) * line()); }

template <int linesize>
void ScanlineContainer<linesize>::clear() noexcept { fill(0u); }

template <int linesize>
void ScanlineContainer<linesize>::fill(const color value) noexcept { m_data.fill(value); }

template <int linesize>
void ScanlineContainer<linesize>::fill(const Enum::Position position, const color value) noexcept {
	auto colors = data(position);

	for (auto i = 0u; i < size(); ++i)
		colors[i] = value;
}

template <int linesize>
constexpr auto ScanlineContainer<linesize>::fromIndexToPosition(const std::size_t index) noexcept { return static_cast<Enum::Position>(index / ScanlineContainer::line()); }

template <int linesize>
void ScanlineContainer<linesize>::operator=(const ScanlineContainer &other) noexcept { m_data = other.m_data; }

template <int linesize>
void ScanlineContainer<linesize>::operator=(const color value) noexcept { fill(value); }

template <int linesize>
bool ScanlineContainer<linesize>::operator==(const ScanlineContainer &other) const noexcept { return m_data == other.m_data; }

template <int linesize>
bool ScanlineContainer<linesize>::operator!=(const ScanlineContainer &other) const noexcept { return !operator==(other); }

template <int linesize>
void ScanlineContainer<linesize>::operator<<(const color value) noexcept {
	std::rotate(m_data.begin(), m_data.begin() + 1, m_data.end());
	m_data[linesize - 1] = value;
}

template <int linesize>
void ScanlineContainer<linesize>::operator>>(const color value) noexcept {
	std::rotate(m_data.rbegin(), m_data.rbegin() + 1, m_data.rend());
	m_data[0] = value;
}

static_assert(sizeof(Scanline) == Scanline::size() * sizeof(color), "ScanlineContainer should fit in ScanlineSize");
static_assert(alignof(Scanline) == sizeof(color), "ScanlineContainer should be align to sizeof(color)");

template <int linesize>
color ScanlineContainer<linesize>::interpolation(ccolor start, ccolor end, double p) {
	using namespace Functional::Color;
	const auto r = static_cast<ccolor>(getR(end) * p + (getR(start) * (1.0 - p)));
	const auto g = static_cast<ccolor>(getG(end) * p + (getG(start) * (1.0 - p)));
	const auto b = static_cast<ccolor>(getB(end) * p + (getB(start) * (1.0 - p)));
	return rgb(r, g, b);
}

template <int linesize>
void ScanlineContainer<linesize>::interpolate(const ScanlineContainer &start, const ScanlineContainer &end, double p, ScanlineContainer &out) noexcept {
	using namespace Functional::Color;
	for (int i = 0; i < size() - 1; i += 4) {
		out.data()[i + 0] = interpolation(start.constData()[i + 0], end.constData()[i + 0], p);
		out.data()[i + 1] = interpolation(start.constData()[i + 1], end.constData()[i + 1], p);
		out.data()[i + 2] = interpolation(start.constData()[i + 2], end.constData()[i + 2], p);
		out.data()[i + 3] = interpolation(start.constData()[i + 3], end.constData()[i + 3], p);
	}
}

template <int linesize>
template <int newsize>
ScanlineContainer<newsize> ScanlineContainer<linesize>::resize() {
	ScanlineContainer<newsize> result;
	const auto osize = static_cast<float>(linesize);
	const auto nsize = static_cast<float>(newsize);

	for (double i = 0.0; i < osize - 1; i += osize / nsize) {
	}

	return result;
}
}
