#pragma once

#include <core/abstracts/abstract-receiver-manager.h>

#include <functional>
#include <list>
#include <memory>

#include <QObject>
#include <QTimer>

namespace Device {

	class DeviceThread;
	class DevicePort;

	class DeviceManager : public QObject, public Abstract::AbstractReceiverManager {
		Q_OBJECT
	public:
		explicit DeviceManager(QObject *parent = nullptr);
		virtual ~DeviceManager();

		void setRegisterDeviceCallback(const std::function<bool (Interface::IReceiver *, const QString &serialNumber)> &callback);

		DeviceThread *primary();

	protected:
		void remove();
		void rescan();

	private:
		std::function<bool(Interface::IReceiver *, const QString &serialNumber)> m_registerDeviceCallback;
		QTimer m_deviceScan;
	};

}
