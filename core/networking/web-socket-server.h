#pragma once

#include "core/interfaces/imulti-notifier-manager.h"
#include "core/interfaces/iremote-controller.h"

#include <memory>
#include <list>

class QWebSocket;
class QWebSocketServer;

namespace Network {

class WebSocketConnection;

class WebSocketConnectionManager final {
public:
	explicit WebSocketConnectionManager(Interface::IMutliNotifierManager &notifier, Interface::IRemoteController &remoteController, const uint16_t &port = 4999);
	virtual ~WebSocketConnectionManager();

	bool isListening() const noexcept;
	uint16_t port() const noexcept;

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
