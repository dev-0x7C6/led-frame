#pragma once

#include "core/interfaces/imulti-notifier.h"

#include <QObject>

class QWebSocket;

namespace Network {

class WebSocket : public QObject, public Interface::IMultiNotifier {
	Q_OBJECT
public:
	explicit WebSocket(QWebSocket *socket, QObject *parent = nullptr);
	virtual ~WebSocket() = default;

	void send(const QString &message);

	virtual void attached(Corrector::Interface::ICorrector *corrector) override;
	virtual void detached(Corrector::Interface::ICorrector *corrector) override;
	virtual void modified(Corrector::Interface::ICorrector *corrector) override;

	virtual void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	virtual void attached(Receiver::Interface::IReceiver *receiver) override;
	virtual void detached(Receiver::Interface::IReceiver *receiver) override;
	virtual void modified(Receiver::Interface::IReceiver *receiver) override;

private:
	QWebSocket *m_webSocket;

signals:
	void textMessageReceived(const QString &message);
};
}
