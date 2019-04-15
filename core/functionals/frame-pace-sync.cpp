#include <core/functionals/frame-pace-sync.h>
#include <unistd.h>

#include <thread>

using namespace Functional;

using namespace std::chrono;

FramePaceSync::FramePaceSync(const u64 hz)
		: m_point(high_resolution_clock::now())
		, m_perfectFrameTiming(nanoseconds(std::nano::den / hz))
		, m_hz(hz) {
}

void FramePaceSync::synchronize() {
	std::this_thread::sleep_for(m_perfectFrameTiming - duration_cast<nanoseconds>(high_resolution_clock::now() - m_point));
	m_point = high_resolution_clock::now();
}
