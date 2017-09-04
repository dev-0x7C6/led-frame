#pragma once

#include <core/interfaces/iremote-controller.h>

#include <memory>

class QWebSocket;

namespace Network {

class WebSocketConnection final : public INotification {
public:
	explicit WebSocketConnection(Interface::IRemoteController &remoteController, std::unique_ptr<QWebSocket> &&socket);
	virtual ~WebSocketConnection();

protected:
	virtual void action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept override;


protected:
	void recv(const QString &message);
	void send(const QString &message);

private:
	Interface::IRemoteController &m_remoteController;
	std::unique_ptr<QWebSocket> m_socket;
};
}
