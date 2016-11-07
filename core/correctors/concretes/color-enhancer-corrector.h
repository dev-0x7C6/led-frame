#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class ColorEnhancerCorrector final : public Abstract::AbstractCorrector {
public:
	explicit ColorEnhancerCorrector(const int owner);
	virtual ~ColorEnhancerCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(uint32_t color) override;

	virtual double minimumFactor() const override { return 1; }
	virtual double maximumFactor() const override { return 6; }
};
}
}
