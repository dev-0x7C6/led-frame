#include <core/functionals/loop-sync.h>
#include <unistd.h>

using namespace Functional;

FramePaceSync::FramePaceSync(const u32 hz)
		: m_hz(hz)
		, m_runtime(0)
		, m_alltime(0)
		, m_loop(0) {
	m_elapsed.start();
}

u32 FramePaceSync::wait() {
	m_runtime = m_elapsed.elapsed();
	m_alltime += m_runtime;
	m_elapsed.restart();
	double delay = (1000000 / static_cast<double>(m_hz)) - m_runtime;

	if (delay < 0)
		delay = 0.0;

	usleep(delay);
	m_alltime += delay;

	if (m_alltime >= 1000000) {
		m_elapsed.restart();
		m_alltime = 0;
		m_loop++;
	}

	return m_loop;
}

u32 FramePaceSync::loopCount() {
	return m_loop;
}
