#pragma once

#include <array>
#include <cmath>
#include <core/types.h>

#include <core/containers/color-scanline-container.h>

namespace Functional {

struct Block {
	u32 scanline;
	u32 width;
	u32 height;
	u32 step;
	u32 wdiff;
	u32 hdiff;
};

template <typename type, u32 size>
inline auto scan_all(ccolor *data, const Block &block) {
	const u32 memory_shift = block.scanline * (block.step - 1) + block.wdiff;
	std::array<type, size> result;

	for (u32 y = 0u; y < block.height; y += block.step) {
		for (u32 column = 0u; column < size; ++column) {
			type avg;
			for (u32 x = 0u; x < block.width; x += block.step)
				avg += data[x];
			result[column] += avg;
			data += block.width;
		}
		data += memory_shift;
	}

	return result;
}

template <typename type, u32 size>
inline auto scan_edge(ccolor *data, const Block &block) {
	std::pair<type, type> result;

	for (u32 y = 0u; y < block.height; y += block.step) {
		for (u32 x = 0u; x < block.width; x += block.step)
			result.first += data[x];

		data += block.width * (size - 1);
		for (u32 x = 0u; x < block.width; x += block.step)
			result.second += data[x];

		data += block.width + block.scanline * (block.step - 1) + block.wdiff;
	}

	return result;
}

template <class type, u32 rows, u32 columns>
class ImageBlockProcessor final {
public:
	void process(ccolor *const data, u32 width, u32 height, u32 step = 0) {
		Block block;
		block.scanline = width;
		block.width = width / columns;
		block.height = height / rows;
		block.step = (step == 0) ? std::max(1u, static_cast<u32>(std::sqrt(std::sqrt(block.width * block.height)))) : step;
		block.wdiff = width - (block.width * columns);
		block.hdiff = height - (block.height * rows);

		std::array<type, rows - 2> l;
		std::array<type, rows - 2> r;

		auto t = scan_all<type, columns>(data, block);

		for (u32 i = 0; i < rows - 2; ++i) {
			auto ret = scan_edge<type, columns>(data + ((i + 1) * block.scanline * block.height) + block.hdiff / 2 * block.scanline, block);
			l[i] = ret.first;
			r[i] = ret.second;
		}

		auto b = scan_all<type, columns>(data + ((rows - 1) * block.scanline * block.height) + block.hdiff * block.scanline, block);

#ifdef RPI
		auto tc = Container::createInterpolatedColorArray<columns, 32>([&t](cu32 index) { return t.at(index).bgr(); });
		auto bc = Container::createInterpolatedColorArray<columns, 32>([&b](cu32 index) { return b.at(index).bgr(); });
		auto lc = Container::createInterpolatedColorArray<rows - 2, 32>([&l](cu32 index) { return l.at(index).bgr(); });
		auto rc = Container::createInterpolatedColorArray<rows - 2, 32>([&r](cu32 index) { return r.at(index).bgr(); });
#else
		auto tc = Container::createInterpolatedColorArray<columns, 32>([&](cu32 index) { return t.at(index)(); });
		auto bc = Container::createInterpolatedColorArray<columns, 32>([&](cu32 index) { return b.at(index)(); });
		auto lc = Container::createInterpolatedColorArray<rows - 2, 32>([&](cu32 index) { return l.at(index)(); });
		auto rc = Container::createInterpolatedColorArray<rows - 2, 32>([&](cu32 index) { return r.at(index)(); });
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
