#include <core/correctors/concretes/brightness-corrector.h>

using namespace Enum;
using namespace Corrector::Concrete;

BrightnessCorrector::BrightnessCorrector()
		: Abstract::AbstractCorrector(0.5) {
}

CorrectorType BrightnessCorrector::type() {
	return CorrectorType::Brightness;
}

uint32_t BrightnessCorrector::correct(const uint32_t &color) {
	auto r = static_cast<uint32_t>((color >> 0x10) & 0xffu);
	auto g = static_cast<uint32_t>((color >> 0x08) & 0xffu);
	auto b = static_cast<uint32_t>((color >> 0x00) & 0xffu);
	const float brightness = factor();
	r *= brightness;
	g *= brightness;
	b *= brightness;
	r = std::min(0xffu, r);
	g = std::min(0xffu, g);
	b = std::min(0xffu, b);
	uint32_t base = 0;
	base |= r << 0x10;
	base |= g << 0x08;
	base |= b << 0x00;
	return base;
}
