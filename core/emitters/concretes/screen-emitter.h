#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <atomic>
#include <QThread>

namespace Emitter {
namespace Concrete {

class ScreenEmitter final : public QThread, public Abstract::AbstractEmitter {
public:
	explicit ScreenEmitter(ci32 id, ci32 screenId = 0);
	virtual ~ScreenEmitter();

	virtual EmitterType type() const override;

protected:
	void interrupt();
	void run() override;

private:
	std::atomic<bool> m_interrupted;
	ci32 m_screenId;
};
} // namespace Concrete
} // namespace Emitter
