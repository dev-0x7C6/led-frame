#pragma once

#include <core/correctors/abstracts/abstract-corrector.h>

namespace Corrector {
namespace Concrete {

class RGBChannelCorrector final : public Abstract::AbstractCorrector {
public:
	explicit RGBChannelCorrector();
	virtual ~RGBChannelCorrector() = default;

	virtual Enum::CorrectorType type() override;
	virtual uint32_t correct(const uint32_t &color) override;

	float redFactor() const;
	float greenFactor() const;
	float blueFactor() const;

	void setRedFactor(float factor);
	void setGreenFactor(float factor);
	void setBlueFactor(float factor);

private:
	std::atomic<float> m_rfactor;
	std::atomic<float> m_gfactor;
	std::atomic<float> m_bfactor;
};
}
}
