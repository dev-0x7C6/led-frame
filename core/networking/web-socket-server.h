#pragma once

#include <QObject>

class QWebSocketServer;

namespace Network {

	class WebSocketServer final : public QObject {
	public:
		explicit WebSocketServer(const uint16_t &port = 4999, QObject *parent = nullptr);
		virtual ~WebSocketServer() = default;

		bool isListening() const;
		uint16_t port() const;

	protected:
		void incommingConnection();

	private:
		QWebSocketServer *m_webSocketServer;
	};

}
