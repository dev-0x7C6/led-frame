#pragma once

#include <QObject>

#include <core/emitters/abstracts/abstract-emitter.h>

namespace Emitter {
namespace Concrete {

class TestEmitter final : public Abstract::AbstractEmitter {
public:
	explicit TestEmitter();
	virtual ~TestEmitter() = default;

	virtual Enum::EmitterType type() const override;

protected:
	virtual void timerEvent(QTimerEvent *event) override;
	virtual void onConnect(const uint32_t &count) override;
	virtual void onDisconnect(const uint32_t &count) override;

private:
	uint8_t m_currentStep;
};
}
}
