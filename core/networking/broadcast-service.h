#pragma once

#include <memory>
#include <QObject>

class QUdpSocket;

namespace Network {

class BroadcastService final : public QObject {
	Q_OBJECT
public:
	explicit BroadcastService(const int deviceId, const QString &deviceName, const uint16_t &port = 4999, QObject *parent = nullptr);
	virtual ~BroadcastService();

	uint16_t servicePort() const;

protected:
	virtual void timerEvent(QTimerEvent *event) override;

private:
	std::unique_ptr<QUdpSocket> m_socket;
	int m_deviceId;
	const QString m_deviceName;
	const uint16_t m_servicePort;
};
}
