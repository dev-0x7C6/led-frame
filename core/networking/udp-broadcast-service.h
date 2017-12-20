#pragma once

#include <QString>
#include <memory>

#include <core/types.h>

class QUdpSocket;
class QTimer;

namespace Network {

class UdpBroadcastService final {
public:
	explicit UdpBroadcastService(const int deviceId, const std::string &deviceName, cu16 port = 4999);
	virtual ~UdpBroadcastService();

	int id() const { return m_deviceId; }
	u16 servicePort() const;

protected:
	void broadcast();

private:
	std::unique_ptr<QUdpSocket> m_socket;
	std::unique_ptr<QTimer> m_timer;
	const int m_deviceId;
	const std::string m_deviceName;
	const u16 m_servicePort;
};
}
