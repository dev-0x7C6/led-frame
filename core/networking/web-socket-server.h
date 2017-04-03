#pragma once

#include <core/interfaces/imulti-notifier-manager.h>
#include <core/interfaces/iremote-controller.h>

#include <memory>
#include <list>

class QWebSocket;
class QWebSocketServer;

namespace Network {

class WebSocketConnection;

class WebSocketServer final {
public:
	explicit WebSocketServer(Interface::IMutliNotifierManager &notifier, Interface::IRemoteController &remoteController, const u16 &port = 4999);
	virtual ~WebSocketServer();

	bool isListening() const noexcept;
	u16 port() const noexcept;

protected:
	void incommingConnection();

private:
	std::unique_ptr<QWebSocketServer> m_service;
	std::list<std::unique_ptr<WebSocketConnection>> m_connections;

private:
	Interface::IMutliNotifierManager &m_notifier;
	Interface::IRemoteController &m_remoteController;
};
}
