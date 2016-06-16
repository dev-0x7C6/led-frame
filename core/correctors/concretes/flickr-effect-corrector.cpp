#include <core/correctors/concretes/flickr-effect-corrector.h>

using namespace Corrector::Concrete;

FlickrEffectCorrector::FlickrEffectCorrector() {
}

Enum::CorrectorType FlickrEffectCorrector::type() { return Enum::CorrectorType::FlickrEffect; }

uint32_t FlickrEffectCorrector::correct(uint32_t color) { return (m_skip) ? 0 : color; }

void FlickrEffectCorrector::push() {
	if (m_duration > factor()) {
		m_skip = !m_skip;
		m_duration = 0;
	}
}

void FlickrEffectCorrector::pop() { m_duration += 1; }
