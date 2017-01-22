#include <core/correctors/concretes/flickr-effect-corrector.h>

using namespace Corrector::Concrete;

FlickrEffectCorrector::FlickrEffectCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner, 0, 0, 0, 30) {
	setEnabled(false);
}

Enum::CorrectorType FlickrEffectCorrector::type() const { return Enum::CorrectorType::FlickrEffect; }

color FlickrEffectCorrector::correct(ccolor value) const noexcept { return (m_skip) ? 0 : value; }

void FlickrEffectCorrector::push() {
	if (m_duration > factor()) {
		m_skip = !m_skip;
		m_duration = 0;
	}
}

void FlickrEffectCorrector::pop() { m_duration += 1; }
