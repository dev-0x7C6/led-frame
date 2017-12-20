#include <core/correctors/concretes/brightness-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional;

BrightnessCorrector::BrightnessCorrector(ci32 id, int owner)
		: ICorrector(id, owner) {
}

CorrectorType BrightnessCorrector::type() const { return CorrectorType::Brightness; }

void BrightnessCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();
	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = static_cast<ccolor>(get_r24(value) * f);
		const auto g = static_cast<ccolor>(get_g24(value) * f);
		const auto b = static_cast<ccolor>(get_b24(value) * f);
		value = rgb(r, g, b);
	}
}
