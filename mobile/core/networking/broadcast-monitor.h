#pragma once

#include <QObject>
#include <QVariant>

class QUdpSocket;

namespace Network {

class BroadcastMonitor final : public QObject {
	Q_OBJECT
public:
	explicit BroadcastMonitor(QObject *parent = nullptr);

protected:
	void readPendingDatagrams();

private:
	QUdpSocket *m_socket;
	QList<QVariant> m_devices;

signals:
	void deviceDetected(const QVariant &arg);
};
} // namespace Network
