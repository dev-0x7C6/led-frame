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
	if (factor().isMinimum())
		return;

	scanline.modify([](auto &&r, auto &&g, auto &&b) {
		const auto sum = r + g + b;

		if (sum >= 256) {
			const auto factor = static_cast<double>(sum) / 256.0;
			r = static_cast<u32>(r / factor);
			g = static_cast<u32>(g / factor);
			b = static_cast<u32>(b / factor);
		}
	});
}
