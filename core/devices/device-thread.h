#pragma once

#include <classes/alc-color-samples.h>
#include <classes/alc-runtime-sync.h>
#include <connector/alc-receiver.h>
#include <core/abstracts/abstract-receiver.h>
#include <core/enums/color-format-enum.h>

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

namespace Device {

	class DevicePort;

	class DeviceThread : public QThread, public Abstract::AbstractReceiver {
		Q_OBJECT
	public:
		explicit DeviceThread(std::unique_ptr<DevicePort> &&device, QSerialPortInfo details, QObject *parent = 0);
		virtual ~DeviceThread();

		virtual QString name() const override;
		virtual Enum::ReceiverType type() const override;

		QSerialPortInfo details();
		void interrupt();

	protected:
		virtual void run() override;

	private:
		std::unique_ptr<DevicePort> m_device;
		QSerialPortInfo m_details;
		std::atomic<bool> m_interrupt;

	};

}
