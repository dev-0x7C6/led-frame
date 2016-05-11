#include <core/networking/web-socket-server.h>

#include <QWebSocketServer>
#include <QWebSocket>

using namespace Network;

WebSocketServer::WebSocketServer(const uint16_t &port, QObject *parent)
	: QObject(parent)
	, m_webSocketServer(new QWebSocketServer("LedFrameRemote", QWebSocketServer::NonSecureMode, this))

{
	m_webSocketServer->listen(QHostAddress::Any, port);
	connect(m_webSocketServer, &QWebSocketServer::newConnection, this, &WebSocketServer::incommingConnection);
}

bool WebSocketServer::isListening() const {
	return m_webSocketServer->isListening();
}

uint16_t WebSocketServer::port() const {
	return m_webSocketServer->serverPort();
}

void WebSocketServer::incommingConnection() {
	m_webSocketServer->nextPendingConnection();
}
