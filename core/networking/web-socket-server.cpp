#include "core/networking/web-socket-server.h"
#include "core/networking/web-socket.h"

#include <QWebSocketServer>
#include <QWebSocket>

using namespace Network;

WebSocketConnectionManager::WebSocketConnectionManager(Interface::IMutliNotifierManager &notifier, Interface::IRemoteController &remoteController, const uint16_t &port)
		: m_service(std::make_unique<QWebSocketServer>("LedFrameRemote", QWebSocketServer::NonSecureMode, nullptr))
		, m_notifier(notifier)
		, m_remoteController(remoteController)

{
	m_service->listen(QHostAddress::Any, port);
	QObject::connect(m_service.get(), &QWebSocketServer::newConnection, [this] { incommingConnection(); });
}

WebSocketConnectionManager::~WebSocketConnectionManager() {
	for (const auto &connection : m_connections)
		m_notifier.detach(*connection.get());
}

bool WebSocketConnectionManager::isListening() const noexcept {
	return m_service->isListening();
}

uint16_t WebSocketConnectionManager::port() const noexcept {
	return m_service->serverPort();
}

void WebSocketConnectionManager::incommingConnection() {
	auto connection = std::make_unique<WebSocketConnection>(m_remoteController, std::unique_ptr<QWebSocket>(m_service->nextPendingConnection()));
	m_notifier.attach(*connection.get());
	m_connections.emplace_back(std::move(connection));
}
