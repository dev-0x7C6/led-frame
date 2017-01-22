#include <core/correctors/concretes/flickr-effect-corrector.h>

using namespace Corrector::Concrete;

FlickrEffectCorrector::FlickrEffectCorrector(ci32 id, int owner)
		: Interface::ICorrector(id, owner, 0, 0, 0, 30) {
	setEnabled(false);
}

Enum::CorrectorType FlickrEffectCorrector::type() const { return Enum::CorrectorType::FlickrEffect; }

void FlickrEffectCorrector::correct(Container::Scanline &scanline) const noexcept {
	if (m_duration > factor()) {
		m_skip = !m_skip;
		m_duration = 0;
	}

	for (auto &value : scanline.array())
		value = (m_skip) ? 0 : value;

	m_duration += 1;
}
