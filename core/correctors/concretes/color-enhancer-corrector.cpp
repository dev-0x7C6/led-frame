#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <algorithm>

using namespace Enum;
using namespace Corrector::Concrete;

ColorEnhancerCorrector::ColorEnhancerCorrector(cint id, int owner)
		: Abstract::AbstractCorrector(id, owner, 1.5, 20) {
}

Enum::CorrectorType ColorEnhancerCorrector::type() const { return CorrectorType::ColorEnhancer; }

uint32_t ColorEnhancerCorrector::correct(color value) {
	std::array<decltype(value), 3> colors = {{
		static_cast<decltype(value)>((value >> 0x10) & 0xffu),
		static_cast<decltype(value)>((value >> 0x08) & 0xffu),
		static_cast<decltype(value)>((value >> 0x00) & 0xffu),
	}};

	const double factor = m_factor;
	auto pair = std::minmax_element(colors.cbegin(), colors.cend());

	colors[pair.first - colors.cbegin()] /= factor;
	colors[pair.second - colors.cbegin()] *= factor;

	value = 0;
	value |= std::min(0xffu, colors[0]) << 0x10;
	value |= std::min(0xffu, colors[1]) << 0x08;
	value |= std::min(0xffu, colors[2]) << 0x00;
	return value;
}
