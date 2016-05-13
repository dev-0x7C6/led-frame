#pragma once

#include <QObject>

#include <core/interfaces/icorrector.h>

class QWebSocket;

namespace Network {

	class WebSocket : public QObject {
		Q_OBJECT
	public:
		explicit WebSocket(QWebSocket *socket, QObject *parent = nullptr);
		virtual ~WebSocket() = default;

		void sendTextMessage(const QString &message);

	private:
		QWebSocket *m_webSocket;

	signals:
		void textMessageReceived(const QString &message);
	};

}
