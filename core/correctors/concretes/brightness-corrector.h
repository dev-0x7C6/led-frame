#pragma once

#include <core/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class BrightnessCorrector final : public ICorrector {
public:
	explicit BrightnessCorrector(ci32 id, int owner);
	virtual ~BrightnessCorrector() = default;

	virtual CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};
}
}
