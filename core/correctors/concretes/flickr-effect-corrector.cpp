#include <core/correctors/concretes/flickr-effect-corrector.h>

using namespace Corrector::Concrete;

FlickrEffectCorrector::FlickrEffectCorrector(ci32 id, int owner)
		: ICorrector(id, owner, Enum::Priority::Highest) {
	m_factor.setValue(0);
	m_enabled = false;
}

CorrectorType FlickrEffectCorrector::type() const { return CorrectorType::FlickrEffect; }

void FlickrEffectCorrector::correct(Container::Scanline &scanline) const noexcept {
	const auto f = factor().value();

	if (f == 0)
		return;

	if (m_lastFactor != f) {
		m_duration = 0;
		m_lastFactor = f;
	}

	auto skip = ((m_duration % f) + 1) > (f / 2);

	for (auto &value : scanline.array())
		value = (skip) ? 0 : value;

	m_duration++;
}
