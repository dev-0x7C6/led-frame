#include "color-enhancer-corrector.h"

#include <algorithm>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional;

ColorEnhancerCorrector::ColorEnhancerCorrector(int owner)
		: ICorrector(owner, Priority::Higher) {
	m_factor.setValue(0);
	m_enabled = false;
}

void ColorEnhancerCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().min())
		return;

	for (auto &value : scanline.array()) {
		const auto r = static_cast<int>(get_r24(value));
		const auto g = static_cast<int>(get_g24(value));
		const auto b = static_cast<int>(get_b24(value));

		auto sum = r + g + b;

		if (sum >= 256) {
			auto m = static_cast<double>(sum) / 256.0;

			value = rgb(static_cast<int>(r / m), static_cast<int>(g / m), static_cast<int>(b / m));
		}
	}
}
