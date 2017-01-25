#include "backlight-corrector.h"

#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

BacklightCorrector::BacklightCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner, 0) {
	m_factor.setValue(1);
	m_enabled = false;
}
CorrectorType BacklightCorrector::type() const { return CorrectorType::Backlight; }
void BacklightCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == 0)
		return;

	const auto backlight = factor().value();

	for (auto &value : scanline.array()) {
		const auto r = std::max(backlight, getR(value));
		const auto g = std::max(backlight, getG(value));
		const auto b = std::max(backlight, getB(value));

		value = rgb(r, g, b);
	}
}
