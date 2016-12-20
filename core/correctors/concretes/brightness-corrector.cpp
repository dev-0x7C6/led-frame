#include <core/correctors/concretes/brightness-corrector.h>
#include <core/functionals/color-functions.h>

using namespace Enum;
using namespace Corrector::Concrete;
using namespace Functional::Color;

BrightnessCorrector::BrightnessCorrector(ci32 id, int owner)
		: Abstract::AbstractCorrector(id, owner, 1.0) {
}

double BrightnessCorrector::minimumFactor() const { return 0; }
double BrightnessCorrector::maximumFactor() const { return 1.0; }

CorrectorType BrightnessCorrector::type() const { return CorrectorType::Brightness; }

color BrightnessCorrector::correct(color value) {
	const double l = m_factor;
	const auto r = static_cast<ccolor>(getR(value) * l);
	const auto g = static_cast<ccolor>(getG(value) * l);
	const auto b = static_cast<ccolor>(getB(value) * l);
	return rgb(r, g, b);
}
