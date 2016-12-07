#include <core/correctors/concretes/flickr-effect-corrector.h>

using namespace Corrector::Concrete;

FlickrEffectCorrector::FlickrEffectCorrector(ci32 id, int owner)
		: Abstract::AbstractCorrector(id, owner, minimumFactor()) {
	m_enabled = false;
}

Enum::CorrectorType FlickrEffectCorrector::type() const { return Enum::CorrectorType::FlickrEffect; }
double FlickrEffectCorrector::minimumFactor() const { return 0; }
double FlickrEffectCorrector::maximumFactor() const { return 30; }

color FlickrEffectCorrector::correct(color value) { return (m_skip) ? 0 : value; }

void FlickrEffectCorrector::push() {
	if (m_duration > factor()) {
		m_skip = !m_skip;
		m_duration = 0;
	}
}

void FlickrEffectCorrector::pop() { m_duration += 1; }
