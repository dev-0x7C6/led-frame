#pragma once

#include <core/correctors/interfaces/icorrector.h>
#include <atomic>

namespace Corrector {
namespace Abstract {

class AbstractCorrector : public Corrector::Interface::ICorrector {
public:
	explicit AbstractCorrector(ci32 id, int owner, double factor);
	explicit AbstractCorrector(ci32 id, int owner, double factor, u32 priority);
	explicit AbstractCorrector(ci32 id, int owner);
	virtual ~AbstractCorrector() = default;

	virtual double factor() const override;
	virtual u32 priority() const override;

	virtual void push() override;
	virtual void pop() override;

	virtual bool isEnabled() const override;
	virtual void setEnabled(bool isEnabled) override;
	virtual void setFactor(double factor) override;
	virtual void setPriority(u32 priority) override;

protected:
	std::atomic<bool> m_enabled;
	std::atomic<u32> m_priority;
	std::atomic<double> m_factor;
};
}
}
