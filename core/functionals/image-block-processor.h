#pragma once

#include <array>
#include <cmath>
#include <core/types.h>

#include <iostream>

namespace Functional {

template <typename type, u32 rows, u32 columns>
using Matrix = std::array<std::array<type, columns>, rows>;

template <class type, u32 rows, u32 columns>
class ImageBlockProcessor final {
public:
	void process(ccolor *data, u32 w, u32 h, u32 step = 0) {
		constexpr auto columnCount = columns - 1u;
		constexpr auto rowCount = rows - 1u;
		const auto scanline = w;
		const auto bx = w / columns;
		const auto by = h / rows;
		const auto diff = w - (bx * columns);
		w = bx * columns;
		h = by * rows;

		step = (step == 0) ? std::max(1u, static_cast<u32>(std::sqrt(std::sqrt(bx * by)))) : step;

		std::array<type, columns> l;
		std::array<type, columns> r;

		auto scanWhole = [&](cu32 index) noexcept {
			std::array<type, columns> line;
			ccolor *source = data + (index * by * scanline);
			for (u32 y = 0u; y < by; y += step) {
				for (u32 cx = 0u; cx <= columnCount; ++cx) {
					for (u32 x = 0u; x < bx; x += step)
						line[cx] += source[x];
					source += bx;
				}
				source += diff;
			}
			return line;
		};

		auto scanEdge = [&](cu32 index, type &lhs, type &rhs) noexcept {
			const ccolor *source = data + (index * by * scanline);
			const auto skip = bx * columnCount;
			const auto tail = bx + diff;
			for (u32 y = 0u; y < by; y += step) {
				for (u32 x = 0u; x < bx; x += step)
					lhs += source[x];

				source += skip;
				for (u32 x = 0u; x < bx; x += step)
					rhs += source[x];

				source += tail;
			}
		};

		m_t = scanWhole(0u);
		for (u32 i = 1u; i < rowCount; ++i) {
			scanEdge(i, l[i], r[i]);
		}
		m_b = scanWhole(rowCount);

		l[0u] = m_t[0u];
		r[0u] = m_t[columnCount];
		l[columnCount] = m_b[0];
		r[columnCount] = m_b[columnCount];

		m_l = l;
		m_r = r;
	}

	auto columnCount() const noexcept { return columns; }
	auto rowCount() const noexcept { return rows; }

	auto top() const noexcept { return m_t; }
	auto bottom() const noexcept { return m_b; }
	auto left() const noexcept { return m_l; }
	auto right() const noexcept { return m_r; }

private:
	std::array<type, columns> m_t;
	std::array<type, columns> m_b;
	std::array<type, columns> m_l;
	std::array<type, columns> m_r;
};
}
