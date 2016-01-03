#ifndef ALCRUNTIMESYNC_H
#define ALCRUNTIMESYNC_H

#include <QElapsedTimer>

class ALCRuntimeSync {
public:
	ALCRuntimeSync();

	quint64 wait(int hz = 100);
	quint64 loopCount();

private:
	QElapsedTimer m_elapsed;
	quint64 m_runtime;
	quint64 m_alltime;
	quint64 m_loop;
};


#endif // ALCRUNTIMESYNC_H
