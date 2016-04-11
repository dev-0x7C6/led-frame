#pragma once

#include <core/containers/color-correction-container.h>
#include <core/enums/corrector-type.h>

namespace Interface {

	class IColorCorrector {
	public:
		explicit IColorCorrector() = default;
		virtual ~IColorCorrector() = default;

		virtual Enum::CorrectorType type() = 0;

		virtual bool enabled() const = 0;
		virtual uint32_t correct(const uint32_t &color) = 0;
		virtual uint32_t priority() const = 0;


	};

}
