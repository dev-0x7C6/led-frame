#pragma once

#include <array>
#include <cmath>
#include <core/types.h>

#include <core/containers/color-scanline-container.h>

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
		const auto w_diff = w - (bx * columns);
		const auto h_diff = h - (by * rows);
		w = bx * columns;
		h = by * rows;

		step = (step == 0) ? std::max(1u, static_cast<u32>(std::sqrt(std::sqrt(bx * by)))) : step;

		std::array<type, columns> l;
		std::array<type, columns> r;

		auto scanWhole = [&](cu32 index, cu32 skip = 0) noexcept {
			std::array<type, columns> line;
			ccolor *source = const_cast<color *>(data + (index * by * scanline) + skip);
			for (u32 y = 0u; y < by; y += step) {
				for (u32 cx = 0u; cx <= columnCount; ++cx) {
					for (u32 x = 0u; x < bx; x += step)
						line[cx] += source[x];
					source += bx;
				}
				source += w_diff;
				source += scanline * (step - 1);
			}
			return line;
		};

		auto scanEdge = [&](cu32 index, type & lhs, type & rhs, cu32 skip = 0) noexcept {
			ccolor *source = data + (index * by * scanline) + skip;
			for (u32 y = 0u; y < by; y += step) {
				for (u32 x = 0u; x < bx; x += step)
					lhs += source[x];

				source += bx * columnCount;
				for (u32 x = 0u; x < bx; x += step)
					rhs += source[x];

				source += bx + w_diff;
				source += scanline * (step - 1);
			}
		};

		auto t = scanWhole(0u);
		for (u32 i = 0; i < rowCount - 1; ++i)
			scanEdge(i + 1, l[i], r[i], h_diff / 2 * scanline);
		auto b = scanWhole(rowCount, h_diff * scanline);

#ifdef RPI
		auto tc = Container::createInterpolatedColorArray<columns, 32>([&t](cu32 index) { return t.at(index).bgr(); });
		auto bc = Container::createInterpolatedColorArray<columns, 32>([&b](cu32 index) { return b.at(index).bgr(); });
		auto lc = Container::createInterpolatedColorArray<rows - 2, 32>([&l](cu32 index) { return l.at(index).bgr(); });
		auto rc = Container::createInterpolatedColorArray<rows - 2, 32>([&r](cu32 index) { return r.at(index).bgr(); });
#else
		auto tc = Container::createInterpolatedColorArray<columns, 32>([&t](cu32 index) { return t.at(index)(); });
		auto bc = Container::createInterpolatedColorArray<columns, 32>([&b](cu32 index) { return b.at(index)(); });
		auto lc = Container::createInterpolatedColorArray<rows - 2, 32>([&l](cu32 index) { return l.at(index)(); });
		auto rc = Container::createInterpolatedColorArray<rows - 2, 32>([&r](cu32 index) { return r.at(index)(); });
#endif
		for (std::size_t i = 0u; i < 32u; ++i) {
			m_output[i] = lc.at(31 - i);
			m_output[i + 32] = tc.at(i);
			m_output[i + 64] = rc.at(i);
			m_output[i + 96] = bc.at(31 - i);
		}
	}

	auto columnCount() const noexcept { return columns; }
	auto rowCount() const noexcept { return rows; }

	const auto &output() const noexcept { return m_output; }

private:
	Container::Scanline m_output;

	std::array<type, columns> m_t;
	std::array<type, columns> m_b;
	std::array<type, rows - 2> m_l;
	std::array<type, rows - 2> m_r;
};
}
