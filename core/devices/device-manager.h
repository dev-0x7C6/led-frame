#pragma once

#include <QObject>
#include <memory>
#include <list>

namespace Device {

	class DeviceThread;

	class DeviceManager : public QObject {
	public:
		explicit DeviceManager(QObject *parent = 0);
		virtual ~DeviceManager();

	protected:
		void timerEvent(QTimerEvent *event);

	private:
		std::list<std::unique_ptr<DeviceThread>> m_threads;

		int m_timerId;

	};

}
