#pragma once

#include <array>
#include <core/types.h>

namespace Functional {

template <typename type, u32 rows, u32 columns>
using Matrix = std::array<std::array<type, columns>, rows>;

template <class type, u32 rows, u32 columns>
class ImageBlockProcessor final {
public:
	void process(ccolor *data, i32 w, i32 h, i32 step = -1, bool partial = true) {
		clear();
		const auto sx = w / columns;
		const auto sy = h / rows;
		w = sx * columns;
		h = sy * rows;

		if (step == -1)
		{
			step = (sx * sy) / 200;
		}

		for (i32 y = 0; y < h; y += step) {
			const auto py = y / sy;
			auto &row = m_matrix.at(py);

			for (i32 x = 0; x < w; x += step) {
				const auto px = x / sx;
				if (partial && py > 0 && py < (rows - 1) && px > 0 && px < (columns - 1))
					x = sx * 31;

				row.at(px) += data[x];
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
