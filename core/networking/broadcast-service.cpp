#include <core/networking/broadcast-service.h>

#include <QHostInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QTimer>
#include <QUdpSocket>
#include <QObject>

using namespace Network;

BroadcastService::BroadcastService(const int deviceId, const QString &deviceName, const uint16_t &port)
		: m_socket(std::make_unique<QUdpSocket>())
		, m_timer(std::make_unique<QTimer>())
		, m_deviceId(deviceId)
		, m_deviceName(deviceName)
		, m_servicePort(port)

{
	QObject::connect(m_timer.get(), &QTimer::timeout, [this] { broadcast(); });
	m_timer->setInterval(3000);
	m_timer->start();
}

BroadcastService::~BroadcastService() = default;

uint16_t BroadcastService::servicePort() const {
	return m_servicePort;
}

void BroadcastService::broadcast() {
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
