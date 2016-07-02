#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <algorithm>

using namespace Enum;
using namespace Corrector::Concrete;

ColorEnhancerCorrector::ColorEnhancerCorrector()
		: Abstract::AbstractCorrector(3, 20) {
}

Enum::CorrectorType ColorEnhancerCorrector::type() {
	return CorrectorType::ColorEnhancer;
}

uint32_t ColorEnhancerCorrector::correct(uint32_t color) {
	std::array<uint32_t, 3> colors = {{
		static_cast<uint32_t>((color >> 0x10) & 0xffu),
		static_cast<uint32_t>((color >> 0x08) & 0xffu),
		static_cast<uint32_t>((color >> 0x00) & 0xffu),
	}};

	const float factor = m_factor;
	auto pair = std::minmax_element(colors.cbegin(), colors.cend());

//	if ((colors[pair.second - colors.cbegin()] - colors[pair.first - colors.cbegin()]) < 16)
//		return color;

	colors[pair.first - colors.cbegin()] /= factor;
	colors[pair.second - colors.cbegin()] *= factor;

	color = 0;
	color |= std::min(0xffu, colors[0]) << 0x10;
	color |= std::min(0xffu, colors[1]) << 0x08;
	color |= std::min(0xffu, colors[2]) << 0x00;
	return color;
}
