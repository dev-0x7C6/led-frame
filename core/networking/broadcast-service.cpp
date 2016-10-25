#include <core/networking/broadcast-service.h>

#include <QUdpSocket>
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostInfo>

using namespace Network;

BroadcastService::BroadcastService(const int deviceId, const QString &deviceName, const uint16_t &port, QObject *parent)
		: QObject(parent)
		, m_socket(std::make_unique<QUdpSocket>())
		, m_deviceId(deviceId)
		, m_deviceName(deviceName)
		, m_servicePort(port)

{
	startTimer(3000);
}

BroadcastService::~BroadcastService() = default;

uint16_t BroadcastService::servicePort() const {
	return m_servicePort;
}

void BroadcastService::timerEvent(QTimerEvent *event) {
	static_cast<void>(event);
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

	QJsonObject object{
		{"computer", QHostInfo::localHostName()},
		{"id", m_deviceId},
		{"device", m_deviceName},
		{"host", host.toString()},
		{"port", QString::number(m_servicePort)},
	};

	QJsonDocument document(object);
	QByteArray datagram = document.toJson();
	m_socket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);
}
