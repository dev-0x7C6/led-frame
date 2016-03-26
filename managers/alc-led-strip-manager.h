#pragma once

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
