#include <core/correctors/concretes/rgb-channel-corrector.h>

using namespace Enum;
using namespace Corrector::Concrete;

RGBChannelCorrector::RGBChannelCorrector(const int owner)
		: Abstract::AbstractCorrector(owner)
		, m_rfactor(1.0)
		, m_gfactor(1.0)
		, m_bfactor(1.0) {
}

CorrectorType RGBChannelCorrector::type() const { return CorrectorType::RGBChannel; }
double RGBChannelCorrector::minimumFactor() const { return 0; }
double RGBChannelCorrector::maximumFactor() const { return 1.0; }
double RGBChannelCorrector::redFactor() const { return m_rfactor; }
double RGBChannelCorrector::greenFactor() const { return m_gfactor; }
double RGBChannelCorrector::blueFactor() const { return m_bfactor; }

uint32_t RGBChannelCorrector::correct(uint32_t color) {
	auto r = static_cast<uint32_t>((color >> 0x10) & 0xffu);
	auto g = static_cast<uint32_t>((color >> 0x08) & 0xffu);
	auto b = static_cast<uint32_t>((color >> 0x00) & 0xffu);
	r *= m_rfactor;
	g *= m_gfactor;
	b *= m_bfactor;
	r = std::min(0xffu, r);
	g = std::min(0xffu, g);
	b = std::min(0xffu, b);
	color = 0;
	color |= r << 0x10;
	color |= g << 0x08;
	color |= b << 0x00;
	return color;
}

void RGBChannelCorrector::setRedFactor(double factor) {
	if (m_rfactor != factor) {
		m_rfactor = factor;
		notify();
	}
}

void RGBChannelCorrector::setGreenFactor(double factor) {
	if (m_gfactor != factor) {
		m_gfactor = factor;
		notify();
	}
}

void RGBChannelCorrector::setBlueFactor(double factor) {
	if (m_bfactor != factor) {
		m_bfactor = factor;
		notify();
	}
}
