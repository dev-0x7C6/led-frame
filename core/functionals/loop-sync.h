#pragma once

#include <QElapsedTimer>

#include <core/types.h>

namespace Functional {

class FramePaceSync {
public:
	explicit FramePaceSync(u32 hz = 100);
	virtual ~FramePaceSync() = default;

	auto hz() const noexcept { return m_hz; }
	double timing() const noexcept { return (1.0 / m_hz); };

	u32 wait();
	u32 loopCount();

private:
	const u32 m_hz{100};
	QElapsedTimer m_elapsed;
	u32 m_runtime;
	u32 m_alltime;
	u32 m_loop;
};
} // namespace Functional
