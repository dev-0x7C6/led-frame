#pragma once

#include <core/abstracts/abstract-corrector.h>

namespace Corrector {

	class BrightnessCorrector final : public Abstract::AbstractCorrector {
	public:
		explicit BrightnessCorrector();
		virtual ~BrightnessCorrector() = default;

		virtual Enum::CorrectorType type() override;
		virtual uint32_t correct(const uint32_t &color) override;

		float brightness() const;
		void setBrightness(const float &brightness);

	private:
		std::atomic<float> m_brightness;
	};

}
