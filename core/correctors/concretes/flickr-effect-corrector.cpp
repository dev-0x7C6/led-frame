#include <core/correctors/concretes/flickr-effect-corrector.h>

using namespace Corrector::Concrete;

FlickrEffectCorrector::FlickrEffectCorrector(int owner)
		: ICorrector(owner, Enum::Priority::Highest) {
	m_factor.setValue(0);
	m_enabled = false;
}

void FlickrEffectCorrector::correct(Container::Scanline &scanline) const noexcept {
	const auto f = factor().value();

	if (f == 0)
		return;

	if (m_lastFactor != f) {
		m_duration = 0;
		m_lastFactor = f;
	}

	auto skip = ((m_duration % f) + 1) > (f / 2);

	scanline.modify([skip](auto &&r, auto &&g, auto &&b) {
		r = (skip) ? 0 : r;
		g = (skip) ? 0 : g;
		b = (skip) ? 0 : b;
	});

	m_duration++;
}
