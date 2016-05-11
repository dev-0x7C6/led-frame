#include <core/networking/web-socket.h>

#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>

using namespace Network;

WebSocket::WebSocket(Interface::ICorrector *corrector, QWebSocket *socket, QObject *parent)
	: QObject(parent)
	, m_webSocket(socket)
	, m_corrector(corrector) {
	connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocket::textMessageReceived);
}

#include <QDebug>

void WebSocket::textMessageReceived(const QString &message) {
	auto json = QJsonDocument::fromJson(message.toUtf8());
	auto obj = json.object();
	m_corrector->setFactor(obj.value("brightness").toDouble());
}
