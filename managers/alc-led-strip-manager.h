#ifndef ALCLEDSTRIPMANAGER_H
#define ALCLEDSTRIPMANAGER_H

#include <QList>

class ALCStripConfiguration;

class ALCLedStripManager {
private:
	QList <ALCStripConfiguration *> m_configurations;

public:
	explicit ALCLedStripManager();
	virtual ~ALCLedStripManager();

	const QList <ALCStripConfiguration *> &configurations();

	static ALCLedStripManager *instance();

};

#endif // ALCLEDSTRIPMANAGER_H
