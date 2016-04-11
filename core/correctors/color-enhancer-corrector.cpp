#include <core/correctors/color-enhancer-corrector.h>
#include <algorithm>

using namespace Enum;
using namespace Corrector;

ColorEnhancerCorrector::ColorEnhancerCorrector()
	: m_factor(1.2) {
}

Enum::CorrectorType Corrector::ColorEnhancerCorrector::type() {
	return CorrectorType::ColorEnhancer;
}

uint32_t ColorEnhancerCorrector::correct(const uint32_t &color) {
	auto r = static_cast<uint32_t>((color >> 0x10) & 0xFFu);
	auto g = static_cast<uint32_t>((color >> 0x08) & 0xFFu);
	auto b = static_cast<uint32_t>((color >> 0x00) & 0xFFu);
	const double factor = m_factor;

	if (r > g && r > b)
		r *= factor;

	if (g > r && g > b)
		g *= factor;

	if (b > r && b > g)
		b *= factor;

	r = std::min(0xffu, r);
	g = std::min(0xffu, g);
	b = std::min(0xffu, b);
	uint32_t base = 0;
	base |= r << 0x10;
	base |= g << 0x08;
	base |= b << 0x00;
	return base;
}

double ColorEnhancerCorrector::factor() const {
	return m_factor;
}

void ColorEnhancerCorrector::setFactor(const double &factor) {
	m_factor = factor;
}
