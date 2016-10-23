#include <core/correctors/concretes/brightness-corrector.h>

using namespace Enum;
using namespace Corrector::Concrete;

BrightnessCorrector::BrightnessCorrector(const int owner)
		: Abstract::AbstractCorrector(owner, 0.5) {
}

double BrightnessCorrector::minimumFactor() const { return 0; }
double BrightnessCorrector::maximumFactor() const { return 1.0; }

CorrectorType BrightnessCorrector::type() const { return CorrectorType::Brightness; }

uint32_t BrightnessCorrector::correct(uint32_t color) {
	auto r = static_cast<uint32_t>((color >> 0x10) & 0xffu);
	auto g = static_cast<uint32_t>((color >> 0x08) & 0xffu);
	auto b = static_cast<uint32_t>((color >> 0x00) & 0xffu);
	const double brightness = factor();
	r *= brightness;
	g *= brightness;
	b *= brightness;
	r = std::min(0xffu, r);
	g = std::min(0xffu, g);
	b = std::min(0xffu, b);
	color = 0;
	color |= r << 0x10;
	color |= g << 0x08;
	color |= b << 0x00;
	return color;
}
