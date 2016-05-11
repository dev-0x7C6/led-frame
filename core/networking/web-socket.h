#pragma once

#include <QObject>

#include <core/interfaces/icorrector.h>

class QWebSocket;

namespace Network {

	class WebSocket : public QObject {
	public:
		explicit WebSocket(Interface::ICorrector *corrector, QWebSocket *socket, QObject *parent = nullptr);
		virtual ~WebSocket() = default;

	protected:
		void textMessageReceived(const QString &message);

	private:
		QWebSocket *m_webSocket;
		Interface::ICorrector *m_corrector;
	};

}
