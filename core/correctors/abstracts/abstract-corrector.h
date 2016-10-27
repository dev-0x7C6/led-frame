#pragma once

#include <core/correctors/interfaces/icorrector.h>
#include <atomic>

namespace Corrector {
namespace Abstract {

class AbstractCorrector : public Corrector::Interface::ICorrector {
public:
	explicit AbstractCorrector(const int owner, double factor);
	explicit AbstractCorrector(const int owner, double factor, uint32_t priority);
	explicit AbstractCorrector(const int owner);
	virtual ~AbstractCorrector() = default;

	virtual double factor() const override;
	virtual uint32_t priority() const override;

	virtual void push() override;
	virtual void pop() override;

	virtual bool isEnabled() const override;
	virtual void setEnabled(bool isEnabled) override;
	virtual void setFactor(double factor) override;
	virtual void setPriority(uint32_t priority) override;

protected:
	std::atomic<bool> m_enabled;
	std::atomic<uint32_t> m_priority;
	std::atomic<double> m_factor;
};
}
}
