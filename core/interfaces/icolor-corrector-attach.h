#pragma once

#include <core/interfaces/templates/itemplate-attach.h>

namespace Interface {

	class IColorCorrector;

	class IColorCorrectorAttach : public Template::IAttachTemplate<IColorCorrector> {
	public:
		explicit IColorCorrectorAttach() = default;
		virtual ~IColorCorrectorAttach() = default;
	};

}
