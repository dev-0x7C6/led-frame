#pragma once

#include <core/interfaces/templates/itemplate-attach.h>
#include <core/interfaces/icolor-corrector.h>
#include <memory>

namespace Interface {

	class IColorCorrectorAttach {
	public:
		explicit IColorCorrectorAttach() = default;
		virtual ~IColorCorrectorAttach() = default;

		virtual void attach(const std::shared_ptr<IColorCorrector> &pair) = 0;

	protected:
		virtual uint32_t execute(const uint32_t &color) = 0;

	};

}
