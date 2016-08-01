#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class BrightnessCorrector final : public Abstract::AbstractCorrector {
public:
	explicit BrightnessCorrector(const std::string &parent);
	virtual ~BrightnessCorrector() = default;

	virtual double minimumFactor() const override;
	virtual double maximumFactor() const override;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(uint32_t color) override;
};
}
}
