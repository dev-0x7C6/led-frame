#pragma once

#include <core/correctors/interfaces/icorrector.h>

namespace Corrector {
namespace Concrete {

class ColorEnhancerCorrector final : public Interface::ICorrector {
public:
	explicit ColorEnhancerCorrector(ci32 id, int owner);
	virtual ~ColorEnhancerCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual color correct(ccolor value) const noexcept override;
};
}
}
