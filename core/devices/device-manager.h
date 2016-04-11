#pragma once

#include <QObject>

#include <core/interfaces/ireceiver.h>

#include <functional>
#include <list>
#include <memory>

namespace Device {

	class DeviceThread;
	class DevicePort;

	class DeviceManager : public QObject {
	public:
		explicit DeviceManager(QObject *parent = 0);
		virtual ~DeviceManager();

		void setRegisterDeviceCallback(const std::function<bool (Interface::IReceiver *, const QString &serialNumber)> &callback);

		DeviceThread *primary();

	protected:
		void timerEvent(QTimerEvent *event);

	private:
		std::list<std::unique_ptr<DeviceThread>> m_threads;
		std::function<bool(Interface::IReceiver *, const QString &serialNumber)> m_registerDeviceCallback;

	};

}
