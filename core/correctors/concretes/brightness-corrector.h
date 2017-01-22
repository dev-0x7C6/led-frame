#pragma once

#include <core/correctors/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class BrightnessCorrector final : public Interface::ICorrector {
public:
	explicit BrightnessCorrector(ci32 id, int owner);
	virtual ~BrightnessCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual void correct(Container::Scanline &scanline) const noexcept override;
};
}
}
