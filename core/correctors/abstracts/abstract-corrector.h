#pragma once

#include <core/correctors/interfaces/icorrector.h>
#include <atomic>

namespace Corrector {
namespace Abstract {

class AbstractCorrector : public Corrector::Interface::ICorrector {
public:
	explicit AbstractCorrector(float factor);
	explicit AbstractCorrector();
	virtual ~AbstractCorrector() = default;

	virtual bool enabled() const override;
	virtual float factor() const override;
	virtual uint32_t priority() const override;

	virtual void setEnabled(bool enabled) override;
	virtual void setFactor(float factor) override;
	virtual void setPriority(uint32_t priority) override;

protected:
	std::atomic<bool> m_enabled;
	std::atomic<uint32_t> m_priority;
	std::atomic<float> m_factor;
};
}
}
