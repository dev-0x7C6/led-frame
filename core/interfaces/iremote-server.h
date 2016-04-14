#pragma once

#include <QJsonDocument>
#include <QString>

namespace Interface {

	class IRemoteServer {
	public:
		explicit IRemoteServer() = default;
		virtual ~IRemoteServer() = default;

		virtual QString device() = 0;
		virtual bool setDevice(const QString &device) = 0;

		virtual double brightness(double value) = 0;
		virtual void setBrightness(double value) = 0;
	};

}
