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

color ColorEnhancerCorrector::correct(ccolor value) const noexcept {
	using value_t = std::remove_const<decltype(value)>::type;

	std::array<value_t, 3> colors{{
		static_cast<value_t>(getR(value)),
		static_cast<value_t>(getG(value)),
		static_cast<value_t>(getB(value)),
	}};

	const auto f = factor();
	auto pair = std::minmax_element(colors.cbegin(), colors.cend());

	colors[pair.first - colors.cbegin()] /= f;
	colors[pair.second - colors.cbegin()] *= f;

	return rgb(colors.at(0), colors.at(1), colors.at(2));
}
