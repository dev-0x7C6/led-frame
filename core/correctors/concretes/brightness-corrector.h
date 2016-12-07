#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class BrightnessCorrector final : public Abstract::AbstractCorrector {
public:
	explicit BrightnessCorrector(ci32 id, int owner);
	virtual ~BrightnessCorrector() = default;

	virtual double minimumFactor() const override;
	virtual double maximumFactor() const override;

	virtual Enum::CorrectorType type() const override;
	virtual color correct(color value) override;
};
}
}
