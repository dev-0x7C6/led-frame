#include <core/networking/web-socket.h>

#include <QWebSocket>

using namespace Network;

WebSocket::WebSocket(QWebSocket *socket, QObject *parent)
		: QObject(parent)
		, m_webSocket(socket) {
	connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocket::textMessageReceived);
}

void WebSocket::sendTextMessage(const QString &message) {
	m_webSocket->sendTextMessage(message);
}
