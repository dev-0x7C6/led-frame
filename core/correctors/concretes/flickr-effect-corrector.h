#pragma once

#include <core/correctors/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class FlickrEffectCorrector final : public Interface::ICorrector {
public:
	explicit FlickrEffectCorrector(ci32 id, int owner);
	virtual ~FlickrEffectCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;

private:
	mutable int m_duration = 0;
	mutable bool m_skip = true;
};
}
}
