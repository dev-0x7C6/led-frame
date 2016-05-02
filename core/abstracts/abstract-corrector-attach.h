#pragma once

#include <core/interfaces/icolor-corrector-attach.h>
#include <core/interfaces/icolor-corrector.h>

#include <list>
#include <memory>
#include <vector>

namespace Abstract {

	class AbstractCorrectorAttach : public Interface::IColorCorrectorAttach {
	public:
		explicit AbstractCorrectorAttach() = default;
		virtual ~AbstractCorrectorAttach() = default;

		virtual void attach(const std::shared_ptr<Interface::IColorCorrector> &corrector) override;
		virtual const std::vector<std::shared_ptr<Interface::IColorCorrector>> &list() const override;

	protected:
		virtual uint32_t execute(const uint32_t &color) override;

	private:
		std::vector<std::shared_ptr<Interface::IColorCorrector>> m_correctors;

	};

}
