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
	void process(ccolor *data, u32 w, u32 h, u32 step = 0, bool partial = true) {
		clear();

		const auto size = static_cast<u32>(w * h);
		const auto scanline = w;
		const auto bx = w / columns;
		const auto by = h / rows;
		const auto diff = w - (bx * columns);
		w = bx * columns;
		h = by * rows;

		if (step == 0) {
			step = std::max(1u, (bx * by) / 200);
		}

		for (u32 cy = 0u; cy < rows; ++cy) {
			const ccolor *source = data + (cy * by * scanline);
			auto &row = m_matrix[cy];

			if (cy > 0u && cy < rows - 1) {
				for (u32 y = 0u; y < by; y += step) {
					for (u32 x = 0u; x < bx; x += step) {
						row[0] += source[x];
					}

					source += bx * (columns - 1);
					for (u32 x = 0u; x < bx; x += step) {
						row[columns - 1] += source[x];
					}
					source += bx + diff;
				}
			} else {
				for (u32 y = 0u; y < by; y += step) {
					for (u32 cx = 0u; cx < columns; ++cx) {
						for (u32 x = 0u; x < bx; x += step) {
							row[cx] += source[x];
						}
						source += bx;
					}
					source += diff;
				}
			}
		}
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
