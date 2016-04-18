#pragma once

#include <core/abstracts/abstract-receiver.h>
#include <core/containers/device-config-container.h>
#include <core/enums/color-format-enum.h>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

#include <atomic>
#include <memory>

namespace Device {

	class DevicePort;

	class DeviceThread : public QThread, public Abstract::AbstractReceiver {
		Q_OBJECT
	public:
		explicit DeviceThread(std::unique_ptr<DevicePort> &&device, QSerialPortInfo details, QObject *parent = 0);
		virtual ~DeviceThread();

		virtual Enum::ReceiverType type() const override;

		QSerialPortInfo details();
		void interrupt();

		virtual Container::DeviceConfigContainer config() override;

	protected:
		virtual void run() override;

	private:
		std::unique_ptr<DevicePort> m_device;
		QSerialPortInfo m_details;
		std::atomic<bool> m_interrupt;

	};

}
