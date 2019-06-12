#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class FlickrEffectCorrector final : public ICorrector {
public:
	explicit FlickrEffectCorrector(int owner);

	CorrectorType type() const noexcept final { return CorrectorType::FlickrEffect; }
	void correct(Container::Scanline &scanline) const noexcept final;

private:
	mutable u32 m_duration = 0;
	mutable u32 m_lastFactor = 0xffffffffu;
};
} // namespace Concrete
} // namespace Corrector
