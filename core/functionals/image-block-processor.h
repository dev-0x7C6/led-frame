#pragma once

#include <array>
#include "core/types.h"

namespace Functional {

template <typename type, u32 rows, u32 columns>
using Matrix = std::array<std::array<type, columns>, rows>;

template <class type, u32 rows, u32 columns>
class ImageBlockProcessor final {
public:
	void process(ccolor *data, cu32 w, cu32 h, cu32 step = 4, bool partial = true) {
		clear();
		const auto sx = w / columns;
		const auto sy = h / rows;

		for (auto y = 0u; y < h; y += step) {
			const auto py = y / sy;
			auto &row = m_matrix.at(std::min(columns - 1, py));

			for (auto x = 0u; x < w; x += step) {
				const auto px = x / sx;
				if (partial && py > 0 && py < (rows - 1) && px > 0 && px < (columns - 1))
					x = sx * 31;

				row.at(std::min(rows - 1, px)) += data[x];
			}
			data += w * step;
		}
	}

	void clear() {
		for (auto &row : m_matrix)
			for (auto &colorAvg : row)
				colorAvg.clear();
	}

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
