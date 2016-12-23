#include "color-enhancer-corrector.h"

#include <algorithm>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

ColorEnhancerCorrector::ColorEnhancerCorrector(ci32 id, int owner)
		: Abstract::AbstractCorrector(id, owner, 1.0, 20) {
}

Enum::CorrectorType ColorEnhancerCorrector::type() const { return CorrectorType::ColorEnhancer; }

color ColorEnhancerCorrector::correct(color value) {
	using value_t = decltype(value);

	std::array<value_t, 3> colors{{
		static_cast<value_t>(getR(value)),
		static_cast<value_t>(getG(value)),
		static_cast<value_t>(getB(value)),
	}};

	const double factor = m_factor;
	auto pair = std::minmax_element(colors.cbegin(), colors.cend());

	colors[pair.first - colors.cbegin()] /= factor;
	colors[pair.second - colors.cbegin()] *= factor;

	return rgb(colors.at(0), colors.at(1), colors.at(2));
}
