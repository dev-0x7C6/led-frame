#pragma once

#include <QMutexLocker>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

#include <memory>
#include <list>

class ALCDeviceThread;

class ALCDeviceManager : public QObject {
public:
	explicit ALCDeviceManager(QObject *parent = 0);
	virtual ~ALCDeviceManager();

protected:
	void timerEvent(QTimerEvent *event);

private:
	std::list<std::unique_ptr<ALCDeviceThread>> m_deviceThreads;

	int m_timerId;

};
