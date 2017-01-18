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
		const auto wdiff = w - (sx * columns);
		w = sx * columns;
		h = sy * rows;

		if (step == -1) {
			step = (sx * sy) / 200;
		}

		for (i32 y = 0; y < sy; y += step) {
			auto &row = m_matrix[0];
			for (i32 cx = 0; cx < rows; ++cx) {
				for (i32 x = 0; x < sx; x += step) {
					row[cx] += data[x];
				}
				data += sx;
			}
			data += wdiff;
		}
		data += w * (step - 1);

		for (i32 cy = 1; cy < columns - 1; ++cy) {
			for (i32 y = 0; y < sy; y += step) {
				auto &row = m_matrix[cy];

				for (i32 x = 0; x < sx; x += step) {
					row[0] += data[x];
				}
				data += sx * 31;
				for (i32 x = 0; x < sx; x += step) {
					row[31] += data[x];
				}
				data += sx;
				data += wdiff;
			}
			data += w * (step - 1);
		}

		for (i32 y = 0; y < sy; y += step) {
			auto &row = m_matrix[31];
			for (i32 cx = 0; cx < rows; ++cx) {
				for (i32 x = 0; x < sx; x += step) {
					row[cx] += data[x];
				}
				data += sx;
			}
			data += wdiff;
		}

		/*
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
*/
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
