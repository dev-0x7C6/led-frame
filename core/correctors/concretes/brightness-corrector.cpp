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

	scanline.modify([factor{factor().factor()}](auto &&r, auto &&g, auto &&b) {
		r = static_cast<u32>(r * factor);
		g = static_cast<u32>(g * factor);
		b = static_cast<u32>(b * factor);
	});
}
