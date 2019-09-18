#include "backlight-corrector.h"

#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional;

BacklightCorrector::BacklightCorrector(int owner)
		: ICorrector(owner, Priority::Lowest) {
	m_factor = FactorModifier(1, 16, 0);
	m_enabled = false;
}

void BacklightCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == 0)
		return;

	scanline.modify([backlight{static_cast<u32>(factor().value())}](u32 & r, u32 & g, u32 & b) noexcept {
		r = std::max(backlight, r);
		g = std::max(backlight, g);
		b = std::max(backlight, b);
	});
}
