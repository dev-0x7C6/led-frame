#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

class QTimer;

namespace Emitter {
namespace Concrete {

class TestEmitter final : public Abstract::AbstractEmitter {
public:
	explicit TestEmitter(ci32 id);
	~TestEmitter() final;

	EmitterType type() const final { return EmitterType::Test; }

protected:
	void process();

private:
	std::unique_ptr<QTimer> m_timer;
	u8 m_currentStep = 0;
};
} // namespace Concrete
} // namespace Emitter
