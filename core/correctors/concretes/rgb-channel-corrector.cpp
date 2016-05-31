#include <core/correctors/concretes/rgb-channel-corrector.h>

using namespace Enum;
using namespace Corrector::Concrete;

RGBChannelCorrector::RGBChannelCorrector()
		: m_rfactor(1.0)
		, m_gfactor(1.0)
		, m_bfactor(1.0) {
}

CorrectorType RGBChannelCorrector::type() {
	return CorrectorType::RGBChannel;
}

uint32_t RGBChannelCorrector::correct(const uint32_t &color) {
	auto r = static_cast<uint32_t>((color >> 0x10) & 0xffu);
	auto g = static_cast<uint32_t>((color >> 0x08) & 0xffu);
	auto b = static_cast<uint32_t>((color >> 0x00) & 0xffu);
	r *= m_rfactor;
	g *= m_gfactor;
	b *= m_bfactor;
	r = std::min(0xffu, r);
	g = std::min(0xffu, g);
	b = std::min(0xffu, b);
	uint32_t base = 0;
	base |= r << 0x10;
	base |= g << 0x08;
	base |= b << 0x00;
	return base;
}

double RGBChannelCorrector::redFactor() const {
	return m_rfactor;
}

double RGBChannelCorrector::greenFactor() const {
	return m_gfactor;
}

double RGBChannelCorrector::blueFactor() const {
	return m_bfactor;
}

void RGBChannelCorrector::setRedFactor(const float &factor) {
	m_rfactor = factor;
}

void RGBChannelCorrector::setGreenFactor(const float &factor) {
	m_gfactor = factor;
}

void RGBChannelCorrector::setBlueFactor(const float &factor) {
	m_bfactor = factor;
}
