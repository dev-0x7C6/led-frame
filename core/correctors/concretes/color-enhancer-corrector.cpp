#include "color-enhancer-corrector.h"

#include <algorithm>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

ColorEnhancerCorrector::ColorEnhancerCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner, 20) {
	m_factor.setValue(0);
	m_enabled = false;
}

Enum::CorrectorType ColorEnhancerCorrector::type() const { return CorrectorType::ColorEnhancer; }

void ColorEnhancerCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().min())
		return;

	const auto f = m_factor.factor() + 1.0;

	for (auto &value : scanline.array()) {
		std::array<color, 3> colors{{
			static_cast<color>(getR(value)),
			static_cast<color>(getG(value)),
			static_cast<color>(getB(value)),
		}};

		auto pair = std::minmax_element(colors.cbegin(), colors.cend());

		colors[pair.first - colors.cbegin()] /= f;
		colors[pair.second - colors.cbegin()] *= f;

		value = rgb(colors.at(0), colors.at(1), colors.at(2));
	}
}
