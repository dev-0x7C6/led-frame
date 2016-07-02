#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class ColorEnhancerCorrector final : public Abstract::AbstractCorrector {
public:
	explicit ColorEnhancerCorrector();
	virtual ~ColorEnhancerCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(uint32_t color) override;
};
}
}
