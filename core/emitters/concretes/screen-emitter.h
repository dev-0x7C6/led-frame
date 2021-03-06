#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <atomic>
#include <QThread>

namespace Emitter {
namespace Concrete {

class ScreenEmitter final : public QThread, public Abstract::AbstractEmitter {
public:
	explicit ScreenEmitter(i32 screenId = 0);
	~ScreenEmitter() final;

	EmitterType type() const final;

protected:
	void interrupt();
	void run() override;

private:
	std::atomic<bool> m_interrupted;
	const i32 m_screenId;
};
} // namespace Concrete
} // namespace Emitter
