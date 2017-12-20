#include "backlight-corrector.h"

#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional;

BacklightCorrector::BacklightCorrector(ci32 id, int owner)
		: ICorrector(id, owner, Priority::Lowest) {
	m_factor = {1, 16, 0};
	m_enabled = false;
}
CorrectorType BacklightCorrector::type() const { return CorrectorType::Backlight; }
void BacklightCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == 0)
		return;

	const auto backlight = factor().value();

	for (auto &value : scanline.array()) {
		const auto r = std::max(backlight, get_r24(value));
		const auto g = std::max(backlight, get_g24(value));
		const auto b = std::max(backlight, get_b24(value));

		value = rgb(r, g, b);
	}
}
