#pragma once

#include <classes/alc-color-samples.h>
#include <classes/alc-runtime-sync.h>
#include <connector/alc-receiver.h>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

#include <atomic>
#include <memory>

namespace Emitters {
	class ALCEmitter;
}

class ALCColorSamples;
class ALCStripConfiguration;

class ALCDeviceThread : public QThread, public ALCReceiver {
	Q_OBJECT
public:
	explicit ALCDeviceThread(std::unique_ptr<QSerialPort> &&device, QSerialPortInfo details, QObject *parent = 0);
	virtual ~ALCDeviceThread();

	QString name();
	QSerialPortInfo details();

	void interrupt();

protected:
	void run();

private:
	void push(unsigned char *data, quint16 &ptr, Format format,
						quint32 color, Correctors::ALCColorCorrectionValues &values);

private:
	std::unique_ptr<QSerialPort> m_device;
	ALCStripConfiguration *m_config;
	ALCColorSamples m_samples;
	QSerialPortInfo m_details;
	std::atomic<bool> m_interrupt;
};
