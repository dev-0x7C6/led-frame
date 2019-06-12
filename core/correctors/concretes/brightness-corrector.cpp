#include <core/correctors/concretes/brightness-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional;

BrightnessCorrector::BrightnessCorrector(int owner)
		: ICorrector(owner) {
}

CorrectorType BrightnessCorrector::type() const noexcept { return CorrectorType::Brightness; }

void BrightnessCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (factor().value() == factor().max())
		return;

	const auto f = factor().factor();
	for (auto &value : scanline.array()) {
		if (value == 0)
			continue;

		const auto r = static_cast<color>(get_r24(value) * f);
		const auto g = static_cast<color>(get_g24(value) * f);
		const auto b = static_cast<color>(get_b24(value) * f);
		value = rgb(r, g, b);
	}
}
