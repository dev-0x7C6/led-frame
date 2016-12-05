#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class FlickrEffectCorrector final : public Abstract::AbstractCorrector {
public:
	explicit FlickrEffectCorrector(int id, int owner);
	virtual ~FlickrEffectCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(color value) override;
	virtual void push() override;
	virtual void pop() override;

	virtual double minimumFactor() const override;
	virtual double maximumFactor() const override;

private:
	int m_duration = 0;
	bool m_skip = true;
};
}
}
