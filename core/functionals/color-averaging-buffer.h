#pragma once

#include <core/types.h>
#include <core/functionals/color-functions.h>

namespace Functional {

class alignas(4) ColorAveragingBuffer {
public:
	constexpr ColorAveragingBuffer() = default;
	constexpr ColorAveragingBuffer(color r, color g, color b, i32 cnt)
			: m_r(r)
			, m_g(g)
			, m_b(b)
			, m_c(cnt) {
	}

	constexpr void operator+=(color sample) noexcept {
		m_r += get_r24(sample);
		m_g += get_g24(sample);
		m_b += get_b24(sample);
		++m_c;
	}

	constexpr void operator+=(const ColorAveragingBuffer &rhs) noexcept {
		m_r += rhs.m_r;
		m_g += rhs.m_g;
		m_b += rhs.m_b;
		m_c += rhs.m_c;
	}

	constexpr color operator()() const noexcept {
		const auto avg_r = static_cast<color>(m_r / m_c);
		const auto avg_g = static_cast<color>(m_g / m_c);
		const auto avg_b = static_cast<color>(m_b / m_c);
		return rgb(avg_r, avg_g, avg_b);
	}

	constexpr color bgr() const noexcept {
		const auto avg_r = static_cast<color>(m_r / m_c);
		const auto avg_g = static_cast<color>(m_g / m_c);
		const auto avg_b = static_cast<color>(m_b / m_c);
		return rgb(avg_b, avg_g, avg_r);
	}

	constexpr auto count() const noexcept { return m_c; }

private:
	i32 m_r{0};
	i32 m_g{0};
	i32 m_b{0};
	i32 m_c{1};
};

static_assert(is_class_cxx14_efficient_nothrow<ColorAveragingBuffer>::value);
static_assert(alignof(ColorAveragingBuffer));
static_assert(sizeof(ColorAveragingBuffer) == 16);
} // namespace Functional
