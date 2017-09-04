#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class ColorEnhancerCorrector final : public ICorrector {
public:
	explicit ColorEnhancerCorrector(ci32 id, int owner);
	virtual ~ColorEnhancerCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};
}
}
