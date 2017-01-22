#include <core/correctors/concretes/brightness-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

BrightnessCorrector::BrightnessCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner, 1.0) {
}

CorrectorType BrightnessCorrector::type() const { return CorrectorType::Brightness; }

void BrightnessCorrector::correct(Container::Scanline &scanline) const noexcept {
	const auto f = factor();
	for (auto &value : scanline.array()) {
		const auto r = static_cast<ccolor>(getR(value) * f);
		const auto g = static_cast<ccolor>(getG(value) * f);
		const auto b = static_cast<ccolor>(getB(value) * f);
		value = rgb(r, g, b);
	}
}
