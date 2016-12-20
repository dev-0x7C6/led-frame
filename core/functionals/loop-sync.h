#pragma once

#include <QElapsedTimer>

#include <core/types.h>

namespace Functional {

class LoopSync {
public:
	explicit LoopSync();
	virtual ~LoopSync() = default;

	u32 wait(u32 hz = 100);
	u32 loopCount();

private:
	QElapsedTimer m_elapsed;
	u32 m_runtime;
	u32 m_alltime;
	u32 m_loop;
};
}
