#pragma once

#include <core/types.h>
#include <core/functionals/color-functions.h>

namespace Functional {

class alignas(4) ColorAveragingBuffer {
public:
	constexpr ColorAveragingBuffer() = default;
	constexpr ColorAveragingBuffer(color r, color g, color b, u32 cnt)
			: m_r(r)
			, m_g(g)
			, m_b(b)
			, m_counter(cnt) {
	}

	constexpr void operator+=(color sample) noexcept {
		m_r += get_r24(sample);
		m_g += get_g24(sample);
		m_b += get_b24(sample);
		++m_counter;
	}

	constexpr void operator+=(const ColorAveragingBuffer &rhs) noexcept {
		m_r += rhs.m_r;
		m_g += rhs.m_g;
		m_b += rhs.m_b;
		m_counter += rhs.m_counter;
	}

	constexpr color operator()() const noexcept {
		const auto avg_r = std::max(1u, m_r) / std::max(1u, m_counter);
		const auto avg_g = std::max(1u, m_g) / std::max(1u, m_counter);
		const auto avg_b = std::max(1u, m_b) / std::max(1u, m_counter);
		return rgb(avg_r, avg_g, avg_b);
	}

	constexpr color bgr() const noexcept {
		const auto avg_r = std::max(1u, m_r) / std::max(1u, m_counter);
		const auto avg_g = std::max(1u, m_g) / std::max(1u, m_counter);
		const auto avg_b = std::max(1u, m_b) / std::max(1u, m_counter);
		return rgb(avg_b, avg_g, avg_r);
	}

	constexpr auto count() const noexcept { return m_counter; }

private:
	color m_r{0};
	color m_g{0};
	color m_b{0};
	u32 m_counter{0};
};

static_assert(is_class_cxx14_efficient_nothrow<ColorAveragingBuffer>::value);
static_assert(alignof(ColorAveragingBuffer));
static_assert(sizeof(ColorAveragingBuffer) == 16);
} // namespace Functional
