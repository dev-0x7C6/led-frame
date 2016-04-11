#pragma once

#include <core/abstracts/abstract-corrector.h>

namespace Corrector {

	class BrightnessCorrector final : public Abstract::AbstractCorrector {
	public:
		explicit BrightnessCorrector();
		virtual ~BrightnessCorrector() = default;

		virtual Enum::CorrectorType type() override;
		virtual uint32_t correct(const uint32_t &color) override;

		double brightness() const;
		void setBrightness(const double &brightness);

	private:
		std::atomic<double> m_brightness;
	};

}
