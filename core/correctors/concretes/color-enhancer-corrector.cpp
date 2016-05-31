#include <core/correctors/concretes/color-enhancer-corrector.h>
#include <algorithm>

#include <QColor>

using namespace Enum;
using namespace Corrector::Concrete;

ColorEnhancerCorrector::ColorEnhancerCorrector()
		: Abstract::AbstractCorrector(1.5) {
}

Enum::CorrectorType ColorEnhancerCorrector::type() {
	return CorrectorType::ColorEnhancer;
}

uint32_t ColorEnhancerCorrector::correct(const uint32_t &color) {
	auto r = static_cast<uint32_t>((color >> 0x10) & 0xffu);
	auto g = static_cast<uint32_t>((color >> 0x08) & 0xffu);
	auto b = static_cast<uint32_t>((color >> 0x00) & 0xffu);
	auto convert = QColor::fromRgb(r, g, b).toHsl();
	auto buff = QColor::fromHslF(convert.hueF(), std::min(1.0, convert.saturationF() * factor()), convert.lightnessF());
	auto normal = buff.toRgb();
	//  const double factor = m_factor;
	//    if (r > g && r > b)
	//      r *= factor;
	//    if (g > r && g > b)
	//      g *= factor;
	//    if (b > r && b > g)
	//      b *= factor;
	r = std::min(0xffu, static_cast<uint32_t>(normal.red()));
	g = std::min(0xffu, static_cast<uint32_t>(normal.green()));
	b = std::min(0xffu, static_cast<uint32_t>(normal.blue()));
	uint32_t base = 0;
	base |= r << 0x10;
	base |= g << 0x08;
	base |= b << 0x00;
	return base;
}
