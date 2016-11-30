#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

class QTimer;

namespace Emitter {
namespace Concrete {

class TestEmitter final : public Abstract::AbstractEmitter {
public:
	explicit TestEmitter(int id);
	virtual ~TestEmitter();

	virtual Enum::EmitterType type() const override;

protected:
	void process();

protected:
	virtual void onConnect(const uint32_t &count) override;
	virtual void onDisconnect(const uint32_t &count) override;

private:
	std::unique_ptr<QTimer> m_timer;
	uint8_t m_currentStep = 0;
};
}
}
