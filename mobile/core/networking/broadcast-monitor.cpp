#include <core/networking/broadcast-monitor.h>

#include <QJsonDocument>
#include <QUdpSocket>

using namespace Network;

BroadcastMonitor::BroadcastMonitor(QObject *parent)
		: QObject(parent)
		, m_socket(new QUdpSocket(this)) {
	m_socket->bind(45454, QUdpSocket::ShareAddress);
	connect(m_socket, &QUdpSocket::readyRead, this, &BroadcastMonitor::readPendingDatagrams);
}

void BroadcastMonitor::readPendingDatagrams() {
	while (m_socket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(static_cast<int>(m_socket->pendingDatagramSize()));
		m_socket->readDatagram(datagram.data(), datagram.size());
		auto device = QJsonDocument::fromJson(datagram).toVariant();

		if (!device.isValid())
			continue;

		if (!m_devices.contains(device)) {
			emit deviceDetected(device);
			m_devices.append(device);
		}
	}
}
