#pragma once

#include <QObject>

#include <core/interfaces/icorrector.h>
#include <core/interfaces/iemitter-notify.h>

class QWebSocket;

namespace Network {

class WebSocket
	: public QObject,
	  public Interface::IEmitterNotify {
	Q_OBJECT
public:
	explicit WebSocket(QWebSocket *socket, QObject *parent = nullptr);
	virtual ~WebSocket() = default;

	void sendTextMessage(const QString &message);

	virtual void attached(const std::shared_ptr<Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Interface::IEmitter> &emitter) override;

private:
	QWebSocket *m_webSocket;

signals:
	void textMessageReceived(const QString &message);

};
}
