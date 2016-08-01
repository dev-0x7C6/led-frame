#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class FlickrEffectCorrector final : public Abstract::AbstractCorrector {
public:
	explicit FlickrEffectCorrector(const std::string &parent);
	virtual ~FlickrEffectCorrector() = default;

	virtual Enum::CorrectorType type() const override;
	virtual uint32_t correct(uint32_t color) override;
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
