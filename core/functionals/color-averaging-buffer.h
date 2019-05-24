#pragma once

#include <core/types.h>
#include <core/functionals/color-functions.h>

namespace Functional {

template <typename type>
class alignas(4) ColorAveragingBufferTemplate {
public:
	constexpr ColorAveragingBufferTemplate() = default;
	constexpr ColorAveragingBufferTemplate(const type r, const type g, const type b, const u32 cnt)
			: m_r(r)
			, m_g(g)
			, m_b(b)
			, m_c(cnt) {
	}

	constexpr void operator+=(const type sample) noexcept {
		m_r += get_r24(sample);
		m_g += get_g24(sample);
		m_b += get_b24(sample);
		++m_c;
	}

	constexpr void operator+=(const ColorAveragingBufferTemplate &rhs) noexcept {
		m_r += rhs.m_r;
		m_g += rhs.m_g;
		m_b += rhs.m_b;
		m_c += rhs.m_c;
	}

	constexpr type operator()() const noexcept {
		const auto avg_r = static_cast<type>(m_r / m_c);
		const auto avg_g = static_cast<type>(m_g / m_c);
		const auto avg_b = static_cast<type>(m_b / m_c);
		return rgb(avg_r, avg_g, avg_b);
	}

	constexpr type bgr() const noexcept {
		const auto avg_r = static_cast<type>(m_r / m_c);
		const auto avg_g = static_cast<type>(m_g / m_c);
		const auto avg_b = static_cast<type>(m_b / m_c);
		return rgb(avg_b, avg_g, avg_r);
	}

	constexpr auto count() const noexcept { return m_c; }

private:
	type m_r{0};
	type m_g{0};
	type m_b{0};
	u32 m_c{0};
};

using ColorAveragingBuffer = ColorAveragingBufferTemplate<color>;

static_assert(is_class_cxx14_efficient_nothrow<ColorAveragingBuffer>::value);
static_assert(alignof(ColorAveragingBuffer));
static_assert(sizeof(ColorAveragingBuffer) == sizeof(i32) * 4);
} // namespace Functional
