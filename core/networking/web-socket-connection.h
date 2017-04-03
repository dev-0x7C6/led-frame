#pragma once

#include <core/interfaces/imulti-notifier.h>
#include <core/interfaces/iremote-controller.h>

#include <memory>

class QWebSocket;

namespace Network {

class WebSocketConnection : public Interface::IMultiNotifier {
public:
	explicit WebSocketConnection(Interface::IRemoteController &remoteController, std::unique_ptr<QWebSocket> &&socket);
	virtual ~WebSocketConnection();

protected:
	virtual void attached(Corrector::Interface::ICorrector *corrector) override;
	virtual void detached(Corrector::Interface::ICorrector *corrector) override;
	virtual void modified(Corrector::Interface::ICorrector *corrector) override;

	virtual void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	virtual void attached(Receiver::Interface::IReceiver *receiver) override;
	virtual void detached(Receiver::Interface::IReceiver *receiver) override;
	virtual void modified(Receiver::Interface::IReceiver *receiver) override;

protected:
	void recv(const QString &message);
	void send(const QString &message);

private:
	Interface::IRemoteController &m_remoteController;
	std::unique_ptr<QWebSocket> m_socket;
};
}
