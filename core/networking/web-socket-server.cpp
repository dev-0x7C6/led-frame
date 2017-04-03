#include <core/networking/web-socket-server.h>
#include <core/networking/web-socket-connection.h>

#include <QWebSocketServer>
#include <QWebSocket>

using namespace Network;

WebSocketServer::WebSocketServer(Interface::IMutliNotifierManager &notifier, Interface::IRemoteController &remoteController, const u16 &port)
		: m_service(std::make_unique<QWebSocketServer>("LedFrameRemote", QWebSocketServer::NonSecureMode, nullptr))
		, m_notifier(notifier)
		, m_remoteController(remoteController)

{
	m_service->listen(QHostAddress::Any, port);
	QObject::connect(m_service.get(), &QWebSocketServer::newConnection, [this] { incommingConnection(); });
}

WebSocketServer::~WebSocketServer() {
	for (const auto &connection : m_connections)
		m_notifier.detach(*connection.get());
}

bool WebSocketServer::isListening() const noexcept {
	return m_service->isListening();
}

u16 WebSocketServer::port() const noexcept {
	return m_service->serverPort();
}

void WebSocketServer::incommingConnection() {
	auto connection = std::make_unique<WebSocketConnection>(m_remoteController, std::unique_ptr<QWebSocket>(m_service->nextPendingConnection()));
	m_notifier.attach(*connection.get());
	m_connections.emplace_back(std::move(connection));
}
