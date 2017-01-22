#include "color-enhancer-corrector.h"

#include <algorithm>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

ColorEnhancerCorrector::ColorEnhancerCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner, 1.0, 20) {
}

Enum::CorrectorType ColorEnhancerCorrector::type() const { return CorrectorType::ColorEnhancer; }

void ColorEnhancerCorrector::correct(Container::Scanline &scanline) const noexcept {
	for (auto &value : scanline.array()) {
		std::array<color, 3> colors{{
			static_cast<color>(getR(value)),
			static_cast<color>(getG(value)),
			static_cast<color>(getB(value)),
		}};

		const auto f = factor();
		auto pair = std::minmax_element(colors.cbegin(), colors.cend());

		colors[pair.first - colors.cbegin()] /= f;
		colors[pair.second - colors.cbegin()] *= f;

		value = rgb(colors.at(0), colors.at(1), colors.at(2));
	}
}
