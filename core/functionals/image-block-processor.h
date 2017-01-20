#pragma once

#include <array>
#include <core/types.h>

#include <iostream>

namespace Functional {

template <typename type, u32 rows, u32 columns>
using Matrix = std::array<std::array<type, columns>, rows>;

template <class type, u32 rows, u32 columns>
class ImageBlockProcessor final {
public:
	void process(ccolor *data, u32 w, u32 h, u32 step = 0) {
		clear();

		constexpr auto columnCount = columns - 1u;
		constexpr auto rowCount = rows - 1u;
		const auto scanline = w;
		const auto bx = w / columns;
		const auto by = h / rows;
		const auto diff = w - (bx * columns);
		w = bx * columns;
		h = by * rows;

		if (step == 0) {
			step = std::max(1u, (bx * by) / 200);
		}

		auto scanWhole = [&](cu32 index) {
			const ccolor *source = data + (index * by * scanline);
			auto &row = m_matrix[index];

			for (u32 y = 0u; y < by; y += step) {
				for (u32 cx = 0u; cx <= columnCount; ++cx) {
					for (u32 x = 0u; x < bx; x += step) {
						row[cx] += source[x];
					}
					source += bx;
				}
				source += diff;
			}
		};

		auto scanEdge = [&](cu32 index) {
			const ccolor *source = data + (index * by * scanline);
			auto &row = m_matrix[index];

			for (u32 y = 0u; y < by; y += step) {
				for (u32 x = 0u; x < bx; x += step) {
					row[0u] += source[x];
				}

				source += bx * columnCount;
				for (u32 x = 0u; x < bx; x += step) {
					row[columnCount] += source[x];
				}
				source += bx + diff;
			}
		};

		scanWhole(0u);
		for (u32 i = 1u; i < rowCount; ++i)
			scanEdge(i);
		scanWhole(rowCount);
	}

	void clear() {
		for (auto &row : m_matrix)
			for (auto &colorAvg : row)
				colorAvg.clear();
	}

	auto columnCount() const noexcept { return columns; }
	auto rowCount() const noexcept { return rows; }

	const auto &matrix() const noexcept { return m_matrix; }

	color get(std::size_t row, std::size_t column) const noexcept {
#ifdef RPI
		return m_matrix.at(row).at(column).bgr();
#else
		return m_matrix.at(row).at(column)();
#endif
	}

private:
	Matrix<type, rows, columns> m_matrix;
};
}
