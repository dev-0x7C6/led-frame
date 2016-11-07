#include <core/correctors/concretes/brightness-corrector.h>
#include "core/functionals/color-functions.h"

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

BrightnessCorrector::BrightnessCorrector(const int owner)
		: Abstract::AbstractCorrector(owner, 0.5) {
}

double BrightnessCorrector::minimumFactor() const { return 0; }
double BrightnessCorrector::maximumFactor() const { return 1.0; }

CorrectorType BrightnessCorrector::type() const { return CorrectorType::Brightness; }

uint32_t BrightnessCorrector::correct(uint32_t color) {
	const double l = m_factor;
	const auto r = static_cast<uint32_t>(getR(color) * l);
	const auto g = static_cast<uint32_t>(getG(color) * l);
	const auto b = static_cast<uint32_t>(getB(color) * l);
	return rgb(r, g, b);
}
