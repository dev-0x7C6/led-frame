#include "core/networking/web-socket-server.h"
#include "core/networking/web-socket.h"

#include <QWebSocketServer>
#include <QWebSocket>

using namespace Network;

WebSocketConnectionManager::WebSocketConnectionManager(Interface::IMutliNotifierManager &notifier, Interface::IRemoteController &remoteController, const uint16_t &port, QObject *parent)
		: QObject(parent)
		, m_notifier(notifier)
		, m_remoteController(remoteController)
		, m_webSocketServer(new QWebSocketServer("LedFrameRemote", QWebSocketServer::NonSecureMode, this))

{
	m_webSocketServer->listen(QHostAddress::Any, port);
	connect(m_webSocketServer, &QWebSocketServer::newConnection, this, &WebSocketConnectionManager::incommingConnection);
}

bool WebSocketConnectionManager::isListening() const {
	return m_webSocketServer->isListening();
}

uint16_t WebSocketConnectionManager::port() const {
	return m_webSocketServer->serverPort();
}

void WebSocketConnectionManager::incommingConnection() {
	auto connection = new WebSocketConnection(m_remoteController, std::unique_ptr<QWebSocket>(m_webSocketServer->nextPendingConnection()), this);
	m_notifier.attach(*connection);
}
