#pragma once

#include <core/correctors/interfaces/icorrector.h>
#include <atomic>

namespace Corrector {
namespace Abstract {

class AbstractCorrector : public Corrector::Interface::ICorrector {
public:
	explicit AbstractCorrector(double factor);
	explicit AbstractCorrector(double factor, uint32_t priority);
	explicit AbstractCorrector();
	virtual ~AbstractCorrector() = default;

	virtual int id() const override;
	virtual bool enabled() const override;
	virtual double factor() const override;
	virtual uint32_t priority() const override;
	virtual QJsonObject parameters() const override;

	virtual void setEnabled(bool enabled) override;
	virtual void setFactor(double factor) override;
	virtual void setPriority(uint32_t priority) override;

protected:
	std::atomic<uint32_t> m_id;
	std::atomic<bool> m_enabled;
	std::atomic<uint32_t> m_priority;
	std::atomic<double> m_factor;
};
}
}
