#pragma once

#include <core/types.h>
#include <core/containers/color-scanline-container.h>

#include <array>
#include <cmath>
#include <thread>

namespace Functional {

namespace {
struct Block {
	u32 scanline;
	u32 width;
	u32 height;
	u32 step;
	u32 wdiff;
	u32 hdiff;
};
} // namespace

template <typename type, u32 size>
constexpr auto scan_extract(const color *data, const u32 range) noexcept {
	constexpr auto jmp_cacheline = 64 / sizeof(std::remove_pointer_t<std::remove_cvref<decltype(data)>::type>);
	static_assert(jmp_cacheline == 16, "explicit check");

	type buff;

	auto count = (range / jmp_cacheline);

	while (count--) {
		buff += data[0];
		data += jmp_cacheline;
	}

	return buff;
}

template <typename type, u32 size>
constexpr auto scan_all(const color *data, const Block block) noexcept {
	std::array<type, size> result;

	const auto shift = block.scanline * (block.step - 1) + block.wdiff;
	const auto step = block.step;
	const auto w = block.width;
	const auto h = block.height;

	for (auto y = 0u; y < h; y += step) {
		for (auto column = 0u; column < size; ++column) {
			result[column] += scan_extract<type, size>(data, w);
			data += w;
		}
		data += shift;
	}

	return result;
}

template <typename type, u32 size>
constexpr auto scan_edge(const color *data, const Block &block) noexcept {
	const auto step = block.step;
	const auto w = block.width;
	const auto h = block.height;

	type lhs;
	type rhs;

	for (auto y = 0u; y < h; y += step) {
		lhs += scan_extract<type, size>(data, w);
		data += w * (size - 1);
		rhs += scan_extract<type, size>(data, w);
		data += w + block.scanline * (step - 1) + block.wdiff;
	}

	return std::make_pair(lhs, rhs);
}

template <class type, u32 rows, u32 columns>
class ImageBlockProcessor final {
public:
	void process(const color *data, u32 width, u32 height, u32 step = 0) {
		Block block;
		block.scanline = width;
		block.width = width / columns;
		block.height = height / rows;
		block.step = (step == 0) ? std::max(1u, static_cast<u32>(std::sqrt(std::sqrt(block.width * block.height)))) : step;
		block.wdiff = width - (block.width * columns);
		block.hdiff = height - (block.height * rows);

		std::array<std::pair<type, type>, rows - 2> pairs;

		const auto t = scan_all<type, columns>(data, block);

		for (u32 i = 0; i < pairs.size(); ++i)
			pairs[i] = scan_edge<type, columns>(data + ((i + 1) * block.scanline * block.height) + block.hdiff / 2 * block.scanline, block);

		const auto b = scan_all<type, columns>(data + ((rows - 1) * block.scanline * block.height) + block.hdiff * block.scanline, block);

#ifdef RPI
		const auto tc = Container::createInterpolatedColorArray<columns, 32>([&](cu32 index) { return t.at(index).bgr(); });
		const auto bc = Container::createInterpolatedColorArray<columns, 32>([&](cu32 index) { return b.at(index).bgr(); });
		const auto lc = Container::createInterpolatedColorArray<rows - 2, 32>([&](cu32 index) { return pairs.at(index).first.bgr(); });
		const auto rc = Container::createInterpolatedColorArray<rows - 2, 32>([&](cu32 index) { return pairs.at(index).second.bgr(); });
#else
		const auto tc = Container::createInterpolatedColorArray<columns, 32>([&](const u32 index) { return t.at(index)(); });
		const auto bc = Container::createInterpolatedColorArray<columns, 32>([&](const u32 index) { return b.at(index)(); });
		const auto lc = Container::createInterpolatedColorArray<rows - 2, 32>([&](const u32 index) { return pairs.at(index).first(); });
		const auto rc = Container::createInterpolatedColorArray<rows - 2, 32>([&](const u32 index) { return pairs.at(index).second(); });
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
} // namespace Functional
