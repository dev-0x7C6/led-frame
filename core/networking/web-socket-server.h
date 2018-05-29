#pragma once

#include <core/interfaces/iremote-controller.h>
#include <core/interfaces/inotification-aggregator.h>

#include <memory>
#include <list>

class QWebSocket;
class QWebSocketServer;

namespace Network {

class WebSocketConnection;

class WebSocketServer final {
public:
	explicit WebSocketServer(INotificationAggregator &notifier, Interface::IRemoteController &remoteController, const u16 &port = 4999);
	WebSocketServer(const WebSocketServer &) = delete;
	WebSocketServer(WebSocketServer &&) = delete;
	virtual ~WebSocketServer();

	bool isListening() const noexcept;
	u16 port() const noexcept;

	//WebSocketServer& operator=(WebSocketServer const&) = delete;

protected:
	void incommingConnection();

private:
	std::unique_ptr<QWebSocketServer> m_service;
	std::list<std::unique_ptr<WebSocketConnection>> m_connections;

private:
	INotificationAggregator &m_notifier;
	Interface::IRemoteController &m_remoteController;
};
}
