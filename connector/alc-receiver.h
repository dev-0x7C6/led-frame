#ifndef ALCRECEIVER_H
#define ALCRECEIVER_H

#include <QObject>
#include <QMutex>

#include "emitters/alc-emitter.h"

class ALCReceiver {
protected:
	Emitters::ALCEmitter *m_emitter;
	QMutex m_mutex;

public:
	ALCReceiver();

	virtual QString name();

	virtual void connectEmitter(Emitters::ALCEmitter *emitter);
	Emitters::ALCEmitter *connectedEmitter();
};

#endif // ALCRECEIVER_H
