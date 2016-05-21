#pragma once

#include <core/interfaces/icorrector.h>
#include <atomic>

namespace Abstract {

class AbstractCorrector : public Interface::ICorrector {
public:
	explicit AbstractCorrector(const float &factor);
	explicit AbstractCorrector();
	virtual ~AbstractCorrector() = default;

	virtual bool enabled() const override;
	virtual float factor() const override;
	virtual uint32_t priority() const override;

	virtual void setEnabled(const bool &enabled) override;
	virtual void setFactor(const float &factor) override;
	virtual void setPriority(const uint32_t &priority) override;

private:
	std::atomic<bool> m_enabled;
	std::atomic<uint32_t> m_priority;
	std::atomic<float> m_factor;
};
}
