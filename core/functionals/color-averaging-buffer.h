#pragma once

#include <core/types.h>
#include <core/functionals/color-functions.h>

namespace Functional {

class ColorAveragingBuffer {
public:
	constexpr explicit ColorAveragingBuffer() {}

	void clear() noexcept {
		m_r = 0;
		m_g = 0;
		m_b = 0;
		m_counter = 0;
	}

	inline void operator+=(ccolor sample) noexcept {
		m_r += Color::getR(sample);
		m_g += Color::getG(sample);
		m_b += Color::getB(sample);
		++m_counter;
	}

	inline void operator+=(const ColorAveragingBuffer &rhs) noexcept {
		m_r += rhs.m_r;
		m_g += rhs.m_g;
		m_b += rhs.m_b;
		m_counter += rhs.m_counter;
	}

	color operator()() const noexcept {
		using counter_t = decltype(m_counter);
		const auto avg_r = std::max(1u, m_r) / std::max(static_cast<counter_t>(1u), m_counter);
		const auto avg_g = std::max(1u, m_g) / std::max(static_cast<counter_t>(1u), m_counter);
		const auto avg_b = std::max(1u, m_b) / std::max(static_cast<counter_t>(1u), m_counter);
		return Color::rgb(avg_r, avg_g, avg_b);
	}

	color bgr() const noexcept {
		using counter_t = decltype(m_counter);
		const auto avg_r = std::max(1u, m_r) / std::max(static_cast<counter_t>(1u), m_counter);
		const auto avg_g = std::max(1u, m_g) / std::max(static_cast<counter_t>(1u), m_counter);
		const auto avg_b = std::max(1u, m_b) / std::max(static_cast<counter_t>(1u), m_counter);
		return Color::rgb(avg_b, avg_g, avg_r);
	}

	auto count() const noexcept { return m_counter; }

private:
	color m_r = 0;
	color m_g = 0;
	color m_b = 0;
	u32 m_counter = 0;
};
}
