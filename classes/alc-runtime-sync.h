#pragma once

#include <QElapsedTimer>

namespace Functional {

	class LoopSync {
	public:
		explicit LoopSync();
		virtual ~LoopSync() = default;

		quint64 wait(int hz = 100);
		quint64 loopCount();

	private:
		QElapsedTimer m_elapsed;
		quint64 m_runtime;
		quint64 m_alltime;
		quint64 m_loop;
	};

}
