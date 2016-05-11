#include <core/networking/broadcast-service.h>

#include <QTimer>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostInfo>


using namespace Network;

BroadcastService::BroadcastService(const uint16_t &port, QObject *parent)
	: QObject(parent)
	, m_socket(new QUdpSocket)
	, m_servicePort(port)

{
	auto timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &BroadcastService::timeout);
	timer->start(300);
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

	QJsonObject object {
		{"computer", QHostInfo::localHostName()},
		{"host", host.toString()},
		{"port",  QString::number(m_servicePort)}
	};
	QJsonDocument document(object);
	QByteArray datagram = document.toJson();
	m_socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);
}

uint16_t BroadcastService::servicePort() const {
	return m_servicePort;
}

void BroadcastService::setServicePort(const uint16_t &servicePort) {
	m_servicePort = servicePort;
}
