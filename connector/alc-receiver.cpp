#include "connector/alc-receiver.h"

ALCReceiver::ALCReceiver()
	: m_emitter(0) {
}

QString ALCReceiver::name() {
	return QString("default");
}

void ALCReceiver::connectEmitter(Emitters::ALCEmitter *emitter) {
	QMutexLocker locker(&m_mutex);

	if (m_emitter)
		m_emitter->done();

	if ((m_emitter = emitter))
		m_emitter->init();
}

Emitters::ALCEmitter *ALCReceiver::connectedEmitter() {
	QMutexLocker locker(&m_mutex);
	return m_emitter;
}
