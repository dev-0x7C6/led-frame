#pragma once

#include <core/enums/position-enum.h>
#include <core/functionals/color-functions.h>
#include <core/types.h>

#include <algorithm>
#include <array>
#include <functional>
#include <mutex>

namespace Container {

template <u32 linesize>
class alignas(16) ScanlineContainer final {
public:
	static_assert(linesize % 4 == 0);

	explicit ScanlineContainer() noexcept;
	explicit ScanlineContainer(color fillColor) noexcept;

	ScanlineContainer(ScanlineContainer &&) noexcept = default;
	ScanlineContainer(const ScanlineContainer &) noexcept = default;
	ScanlineContainer &operator=(ScanlineContainer &&) noexcept = default;
	ScanlineContainer &operator=(const ScanlineContainer &) noexcept = default;

	constexpr static auto size() noexcept;
	constexpr static auto line() noexcept;

	auto at(std::size_t index) const noexcept;
	auto data() noexcept;
	auto constData() const noexcept;

	auto data(const Enum::Position &position) noexcept;
	auto constData(const Enum::Position &position) const noexcept;

	void clear() noexcept;
	void fill(color value) noexcept;
	void fill(Enum::Position position, color value) noexcept;

	ScanlineContainer &operator=(color value) noexcept;
	bool operator==(const ScanlineContainer &other) const noexcept;
	bool operator!=(const ScanlineContainer &other) const noexcept;
	void operator<<(color value) noexcept;
	void operator>>(color value) noexcept;
	color &operator[](u32 index) noexcept;

	auto &array() noexcept { return m_data; }

	decltype(auto) begin() { return m_data.begin(); }
	decltype(auto) end() { return m_data.end(); }

public:
	constexpr static auto fromIndexToPosition(std::size_t index) noexcept;

	template <u32 newsize>
	inline ScanlineContainer<newsize> resize();

private:
	std::array<color, linesize> m_data;
};

template <u32 linesize>
class SharedScanlinePlaceholderTemplate {
public:
	void set(const ScanlineContainer<linesize> &container) noexcept {
		std::lock_guard _(m_mutex);
		m_scanline = container;
	}

	void set(ScanlineContainer<linesize> &&container) noexcept {
		std::lock_guard _(m_mutex);
		m_scanline = std::move(container);
	}

	ScanlineContainer<linesize> get() const noexcept {
		std::lock_guard _(m_mutex);
		return m_scanline;
	}

private:
	ScanlineContainer<linesize> m_scanline{0};
	mutable std::mutex m_mutex;
};

using Scanline = ScanlineContainer<128u>;
using SharedScanlinePlaceholder = SharedScanlinePlaceholderTemplate<128u>;
static_assert(is_class_cxx14_efficient_nothrow<Scanline>::value);

template <u32 linesize>
inline ScanlineContainer<linesize>::ScanlineContainer() noexcept
		: m_data() {}

template <u32 linesize>
inline ScanlineContainer<linesize>::ScanlineContainer(const color fillColor) noexcept
		: m_data() { m_data.fill(fillColor); }

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::size() noexcept { return linesize; }

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::line() noexcept { return linesize / 4; }

template <u32 linesize>
auto ScanlineContainer<linesize>::at(const std::size_t index) const noexcept {
	return m_data.at(index);
}

template <u32 linesize>
auto ScanlineContainer<linesize>::data() noexcept { return m_data.data(); }

template <u32 linesize>
auto ScanlineContainer<linesize>::constData() const noexcept { return m_data.data(); }

template <u32 linesize>
auto ScanlineContainer<linesize>::data(const Enum::Position &position) noexcept { return m_data.data() + (static_cast<color>(position) * line()); }

template <u32 linesize>
auto ScanlineContainer<linesize>::constData(const Enum::Position &position) const noexcept { return m_data.data() + (static_cast<color>(position) * line()); }

template <u32 linesize>
void ScanlineContainer<linesize>::clear() noexcept { fill(0u); }

template <u32 linesize>
void ScanlineContainer<linesize>::fill(const color value) noexcept { m_data.fill(value); }

template <u32 linesize>
void ScanlineContainer<linesize>::fill(const Enum::Position position, const color value) noexcept {
	auto colors = data(position);

	for (auto i = 0u; i < line(); ++i)
		colors[i] = value;
}

template <u32 linesize>
constexpr auto ScanlineContainer<linesize>::fromIndexToPosition(const std::size_t index) noexcept { return static_cast<Enum::Position>(index / ScanlineContainer::line()); }

template <u32 linesize>
ScanlineContainer<linesize> &ScanlineContainer<linesize>::operator=(const color value) noexcept {
	fill(value);
	return *this;
}

template <u32 linesize>
bool ScanlineContainer<linesize>::operator==(const ScanlineContainer &other) const noexcept { return m_data == other.m_data; }

template <u32 linesize>
bool ScanlineContainer<linesize>::operator!=(const ScanlineContainer &other) const noexcept { return !operator==(other); }

template <u32 linesize>
void ScanlineContainer<linesize>::operator<<(const color value) noexcept {
	std::rotate(m_data.begin(), m_data.begin() + 1, m_data.end());
	m_data[linesize - 1] = value;
}

template <u32 linesize>
void ScanlineContainer<linesize>::operator>>(const color value) noexcept {
	std::rotate(m_data.rbegin(), m_data.rbegin() + 1, m_data.rend());
	m_data[0] = value;
}

template <u32 linesize>
color &ScanlineContainer<linesize>::operator[](u32 index) noexcept {
	return m_data[index];
}

static_assert(sizeof(Scanline) == Scanline::size() * sizeof(color));

constexpr auto linear_interpolation(factor_t start, factor_t end, factor_t factor) noexcept {
	return start + factor * (end - start);
}

constexpr color rgb_linear_interpolation(color start, color end, factor_t p) noexcept {
	using namespace Functional;
	const auto r = static_cast<color>(linear_interpolation(get_r24(start), get_r24(end), p));
	const auto g = static_cast<color>(linear_interpolation(get_g24(start), get_g24(end), p));
	const auto b = static_cast<color>(linear_interpolation(get_b24(start), get_b24(end), p));
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
ScanlineContainer<newsize> ScanlineContainer<linesize>::resize() {
	ScanlineContainer<newsize> result;
	const auto factor = static_cast<float>(linesize) / static_cast<float>(newsize);

	for (int i = 0; i < newsize; ++i) {
		const auto idx = factor * i;
		const auto ret = idx - static_cast<int>(idx);
		result[i] = interpolation(m_data[static_cast<int>(idx)], m_data[static_cast<int>(idx + 1)], ret);
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
