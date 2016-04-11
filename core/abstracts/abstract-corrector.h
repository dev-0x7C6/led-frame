#pragma once

#include <core/interfaces/icolor-corrector.h>
#include <atomic>

namespace Abstract {

	class AbstractCorrector : public Interface::IColorCorrector {
	public:
		explicit AbstractCorrector() = default;
		virtual ~AbstractCorrector() = default;

		virtual bool enabled() const override;
		virtual uint32_t priority() const override;

		void setEnabled(const bool &enabled);
		void setPriority(const uint32_t &priority);

	private:
		std::atomic<bool> m_enabled;
		std::atomic<uint32_t> m_priority;
	};

}
