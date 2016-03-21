#pragma once

#include <QObject>
#include "emitters/alc-emitter.h"

namespace Emitters {
	class ALCAnimationEmitter;
	class ALCColorEmitter;
	class ALCEmitter;
	class ALCImageEmitter;
	class ALCScreenEmitter;
}

class ALCSymulationWidget;
class QSettings;

class ALCEmitterManager : public QObject {
	Q_OBJECT
private:
	QList <Emitters::ALCEmitter *> m_emitters[static_cast<int>(Emitters::ALCEmitter::Type::Last)];
	ALCSymulationWidget *m_symulation;

public:
	explicit ALCEmitterManager(QObject *parent = 0);
	virtual ~ALCEmitterManager();

	void done();

	void add(Emitters::ALCEmitter *emitter, Emitters::ALCEmitter::Type type);
	void remove(Emitters::ALCEmitter *emitter);

	Emitters::ALCAnimationEmitter *addALCAnimationEmitter(const QString &name);
	Emitters::ALCColorEmitter *addALCColorEmitter(const QString &name);
	Emitters::ALCImageEmitter *addALCImageEmitter(const QString &name);
	Emitters::ALCScreenEmitter *addScreenCaptureEmitter(const QString &name);
	void addSymulation(ALCSymulationWidget *widget);

	static ALCEmitterManager *instance();
	const QList <Emitters::ALCEmitter *> *emitters(Emitters::ALCEmitter::Type type);
	const QList <Emitters::ALCEmitter *> allEmitters();

	//private:
	//  void readColorCorrection(QSettings *settings, Correctors::ALCColorCorrection *correction);
	//  void writeColorCorrection(QSettings *, Correctors::ALCColorCorrection *correction);

signals:
	void emitterListChanged();

};
