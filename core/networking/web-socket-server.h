#pragma once

#include "core/interfaces/imulti-notifier-manager.h"
#include "core/interfaces/iremote-controller.h"

#include <QObject>

class QWebSocket;
class QWebSocketServer;

namespace Network {

class WebSocketConnectionManager final : public QObject {
	Q_OBJECT
public:
	explicit WebSocketConnectionManager(Interface::IMutliNotifierManager &notifier, Interface::IRemoteController &remoteController, const uint16_t &port = 4999, QObject *parent = nullptr);
	~WebSocketConnectionManager() override = default;

	bool isListening() const;
	uint16_t port() const;

protected:
	void incommingConnection();

private:
	Interface::IMutliNotifierManager &m_notifier;
	Interface::IRemoteController &m_remoteController;
	QWebSocketServer *m_webSocketServer;

signals:
	void signalIncommingConnection(QWebSocket *socket);
};
}
