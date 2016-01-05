#pragma once

#include <QMutexLocker>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

#include <memory>
#include <list>

class AmbientDeviceThread;

class ALCDeviceManager : public QObject {
public:
	explicit ALCDeviceManager(QObject *parent = 0);
	virtual ~ALCDeviceManager();

protected:
	void timerEvent(QTimerEvent *event);

private:
	std::list<std::unique_ptr<AmbientDeviceThread>> m_deviceThreads;

	int m_timerId;

};
