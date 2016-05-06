#include <core/networking/broadcast-service.h>

#include <QTimer>
#include <QUdpSocket>
#include <QNetworkInterface>

using namespace Network;

BroadcastService::BroadcastService(QObject *parent)
	: QObject(parent)
	, m_socket(new QUdpSocket)
	, m_servicePort(3498)

{
	auto timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &BroadcastService::timeout);
	timer->start(3000);
}

void BroadcastService::timeout() {
	auto interfaces = QNetworkInterface::allInterfaces();
	QHostAddress host;

	for (const auto &interface : interfaces) {
		if (!(interface.flags() & QNetworkInterface::CanBroadcast))
			continue;

		auto addrs = interface.addressEntries();

		for (const auto &addr : addrs) {
			if (addr.ip().isLoopback())
				continue;

			if (addr.ip().protocol() != QAbstractSocket::IPv4Protocol)
				continue;

			host = addr.ip();
		}
	}

	QByteArray datagram = ("ledframe@" + host.toString()
	                       + ":" + QString::number(m_servicePort)).toLocal8Bit();
	m_socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);
}

uint16_t BroadcastService::servicePort() const {
	return m_servicePort;
}

void BroadcastService::setServicePort(const uint16_t &servicePort) {
	m_servicePort = servicePort;
}
