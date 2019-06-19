#pragma once

#include <core/enums/position-enum.h>
#include <core/functionals/color-functions.h>
#include <core/types.h>

#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <cmath>

namespace Container {

template <u32 linesize>
class ScanlineContainer final {
public:
	static_assert(linesize % 4 == 0);

	constexpr explicit ScanlineContainer() noexcept;
	constexpr explicit ScanlineContainer(color fillColor) noexcept;

	constexpr ScanlineContainer(ScanlineContainer &&) noexcept = default;
	constexpr ScanlineContainer(const ScanlineContainer &);
	constexpr ScanlineContainer &operator=(ScanlineContainer &&) noexcept = default;
	constexpr ScanlineContainer &operator=(const ScanlineContainer &) = delete;

	constexpr static auto size() noexcept;
	constexpr static auto line() noexcept;

	constexpr auto at(std::size_t index) const noexcept;
	constexpr auto data() noexcept;
	constexpr auto constData() const noexcept;

	constexpr auto data(const Enum::Position &position) noexcept;
	constexpr auto constData(const Enum::Position &position) const noexcept;

	constexpr void clear() noexcept;
	constexpr void fill(color value) noexcept;
	constexpr void fill(Enum::Position position, color value) noexcept;

	constexpr ScanlineContainer &operator=(color value) noexcept;
	constexpr bool operator==(const ScanlineContainer &other) const noexcept;
	constexpr bool operator!=(const ScanlineContainer &other) const noexcept;
	constexpr void operator<<(color value) noexcept;
	constexpr void operator>>(color value) noexcept;
	constexpr color &operator[](u32 index) noexcept;

	constexpr auto &array() noexcept { return container(); }

	constexpr decltype(auto) begin() { return container().begin(); }
	constexpr decltype(auto) end() { return container().end(); }

public:
	constexpr static auto fromIndexToPosition(std::size_t index) noexcept;

	template <u32 newsize>
	constexpr ScanlineContainer<newsize> resize();

private:
	const auto &container() const noexcept { return *m_data; }
	auto &container() noexcept { return *m_data; }

private:
	std::unique_ptr<std::array<color, linesize>> m_data; //make on heap to faster move
};

using Scanline = ScanlineContainer<128u>;

template <u32 linesize>
constexpr ScanlineContainer<linesize>::ScanlineContainer() noexcept
		: m_data(std::make_unique<std::array<color, linesize>>()) {}

template <u32 linesize>
constexpr ScanlineContainer<linesize>::ScanlineContainer(const color fillColor) noexcept
		: m_data(std::make_unique<std::array<color, linesize>>()) { container().fill(fillColor); }

template <u32 linesize>
constexpr ScanlineContainer<linesize>::ScanlineContainer(const ScanlineContainer &rhs)
		: m_data(std::make_unique<std::array<color, linesize>>()) {
	container() = rhs.container();
}

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::size() noexcept { return linesize; }

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::line() noexcept { return linesize / 4; }

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::at(const std::size_t index) const noexcept {
	return container().at(index);
}

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::data() noexcept { return container().data(); }

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::constData() const noexcept { return container().data(); }

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::data(const Enum::Position &position) noexcept { return container().data() + (static_cast<color>(position) * line()); }

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::constData(const Enum::Position &position) const noexcept { return container().data() + (static_cast<color>(position) * line()); }

template <u32 linesize>
constexpr void ScanlineContainer<linesize>::clear() noexcept { fill(0u); }

template <u32 linesize>
constexpr void ScanlineContainer<linesize>::fill(const color value) noexcept { container().fill(value); }

template <u32 linesize>
constexpr void ScanlineContainer<linesize>::fill(const Enum::Position position, const color value) noexcept {
	auto colors = data(position);

	for (auto i = 0u; i < line(); ++i)
		colors[i] = value;
}

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::fromIndexToPosition(const std::size_t index) noexcept { return static_cast<Enum::Position>(index / ScanlineContainer::line()); }

template <u32 linesize>
constexpr ScanlineContainer<linesize> &ScanlineContainer<linesize>::operator=(const color value) noexcept {
	fill(value);
	return *this;
}

template <u32 linesize>
constexpr bool ScanlineContainer<linesize>::operator==(const ScanlineContainer &other) const noexcept { return container() == other.container(); }

template <u32 linesize>
constexpr bool ScanlineContainer<linesize>::operator!=(const ScanlineContainer &other) const noexcept { return !operator==(other); }

template <u32 linesize>
constexpr void ScanlineContainer<linesize>::operator<<(const color value) noexcept {
	std::rotate(container().begin(), container().begin() + 1, container().end());
	container()[linesize - 1] = value;
}

template <u32 linesize>
constexpr void ScanlineContainer<linesize>::operator>>(const color value) noexcept {
	std::rotate(container().rbegin(), container().rbegin() + 1, container().rend());
	container()[0] = value;
}

template <u32 linesize>
constexpr color &ScanlineContainer<linesize>::operator[](u32 index) noexcept {
	return container()[index];
}

//static_assert(sizeof(Scanline) == Scanline::size() * sizeof(color));

template <typename type>
constexpr auto linear_interpolation(type start, type end, type factor) noexcept {
	return start + factor * (end - start);
}

constexpr color rgb_linear_interpolation(color start, color end, factor_t p) noexcept {
	using namespace Functional;
	const auto r = static_cast<color>(linear_interpolation<factor_t>(get_r24(start), get_r24(end), p));
	const auto g = static_cast<color>(linear_interpolation<factor_t>(get_g24(start), get_g24(end), p));
	const auto b = static_cast<color>(linear_interpolation<factor_t>(get_b24(start), get_b24(end), p));
	return rgb(r, g, b);
}

template <u32 linesize>
ScanlineContainer<linesize> interpolate(const ScanlineContainer<linesize> &start, const ScanlineContainer<linesize> &end, cfactor p) noexcept {
	using namespace Functional;
	ScanlineContainer<linesize> ret;

	for (auto i = 0u; i < linesize; ++i)
		ret.data()[i] = rgb_linear_interpolation(start.constData()[i], end.constData()[i], p);

	return ret;
}

template <u32 linesize>
template <u32 newsize>
constexpr ScanlineContainer<newsize> ScanlineContainer<linesize>::resize() {
	ScanlineContainer<newsize> result;
	const auto factor = static_cast<float>(linesize) / static_cast<float>(newsize);

	for (int i = 0; i < newsize; ++i) {
		const auto idx = factor * i;
		const auto ret = idx - static_cast<int>(idx);
		result[i] = interpolation(container()[static_cast<int>(idx)], container()[static_cast<int>(idx + 1)], ret);
	}

	return result;
}

template <u32 oldsize, u32 newsize>
inline static std::array<color, newsize> createInterpolatedColorArray(const std::function<color(const u32)> &getColor) {
	constexpr auto factor = static_cast<factor_t>(oldsize) / static_cast<factor_t>(newsize);
	std::array<color, newsize> result;

	for (u32 i = 0u; i < newsize; ++i) {
		const auto curr_idx = std::min(oldsize - 1, static_cast<u32>(i * factor));
		const auto next_idx = std::min(oldsize - 1, curr_idx + 1);
		const auto ret = factor * i - static_cast<int>(i * factor);
		result[i] = rgb_linear_interpolation(getColor(curr_idx), getColor(next_idx), ret);
	}

	return result;
}
} // namespace Container
