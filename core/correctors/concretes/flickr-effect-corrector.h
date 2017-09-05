#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class FlickrEffectCorrector final : public ICorrector {
public:
	explicit FlickrEffectCorrector(ci32 id, int owner);
	virtual ~FlickrEffectCorrector() = default;

	virtual CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;

private:
	mutable u32 m_duration = 0;
	mutable u32 m_lastFactor = 0xffffffffu;
};
}
}
