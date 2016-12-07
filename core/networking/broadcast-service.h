#pragma once

#include <QString>
#include <memory>

#include "core/consts.h"

class QUdpSocket;
class QTimer;

namespace Network {

class BroadcastService final {
public:
	explicit BroadcastService(const int deviceId, const QString &deviceName, cu16 port = 4999);
	virtual ~BroadcastService();

	int id() const { return m_deviceId; }
	u16 servicePort() const;

protected:
	void broadcast();

private:
	std::unique_ptr<QUdpSocket> m_socket;
	std::unique_ptr<QTimer> m_timer;
	const int m_deviceId;
	const QString m_deviceName;
	const u16 m_servicePort;
};
}
