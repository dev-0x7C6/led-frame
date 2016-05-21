#pragma once

#include <QObject>

class QWebSocket;
class QWebSocketServer;

namespace Network {

class WebSocketServer final : public QObject {
	Q_OBJECT
public:
	explicit WebSocketServer(const uint16_t &port = 4999, QObject *parent = nullptr);
	virtual ~WebSocketServer() = default;

	bool isListening() const;
	uint16_t port() const;

private:
	QWebSocketServer *m_webSocketServer;

signals:
	void signalIncommingConnection(QWebSocket *socket);
};
}
