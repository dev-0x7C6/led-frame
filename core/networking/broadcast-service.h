#pragma once

#include <QString>
#include <memory>

class QUdpSocket;
class QTimer;

namespace Network {

class BroadcastService final {
public:
	explicit BroadcastService(const int deviceId, const QString &deviceName, const uint16_t &port = 4999);
	virtual ~BroadcastService();

	uint16_t servicePort() const;

protected:
	void broadcast();

private:
	std::unique_ptr<QUdpSocket> m_socket;
	std::unique_ptr<QTimer> m_timer;
	int m_deviceId;
	const QString m_deviceName;
	const uint16_t m_servicePort;
};
}
