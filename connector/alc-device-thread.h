#pragma once

#include <core/abstracts/abstract-receiver.h>
#include <core/enums/color-format-enum.h>
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

class AmbientDeviceThread : public QThread, public Abstract::AbstractReceiver {
	Q_OBJECT
public:
	explicit AmbientDeviceThread(std::unique_ptr<QSerialPort> &&device, QSerialPortInfo details, QObject *parent = 0);
	virtual ~AmbientDeviceThread();

	virtual QString name() const override;
	virtual Enum::ReceiverType type() const override;

	QSerialPortInfo details();

	void interrupt();

protected:
	virtual void run() override;


private:
	std::unique_ptr<QSerialPort> m_device;
	ALCStripConfiguration *m_config;
	ALCColorSamples m_samples;
	QSerialPortInfo m_details;
	std::atomic<bool> m_interrupt;


};
