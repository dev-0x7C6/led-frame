#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class BrightnessCorrector final : public Abstract::AbstractCorrector {
public:
	explicit BrightnessCorrector();
	virtual ~BrightnessCorrector() = default;

	virtual Enum::CorrectorType type() override;
	virtual uint32_t correct(const uint32_t &color) override;
};
}
}
