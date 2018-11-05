#pragma once

#include <core/interfaces/iremote-controller.h>

#include <memory>
#include <queue>
#include <mutex>

class QWebSocket;
class QTimer;

namespace Network {

class WebSocketConnection final : public INotification {
public:
	explicit WebSocketConnection(Interface::IRemoteController &remoteController, std::unique_ptr<QWebSocket> &&socket);

	WebSocketConnection() = delete;
	WebSocketConnection(const WebSocketConnection &) = delete;
	WebSocketConnection &operator=(const WebSocketConnection &) = delete;
	WebSocketConnection(WebSocketConnection &&) noexcept = delete;
	WebSocketConnection &operator=(WebSocketConnection &&) noexcept = delete;
	~WebSocketConnection() final;

	void action(NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept final;

private:
	void recv(const QString &message);
	void send(const QString &message);

private:
	std::mutex m_mutex;
	std::queue<QString> m_outgoingQueue;
	Interface::IRemoteController &m_remoteController;
	std::unique_ptr<QWebSocket> m_socket;

	QTimer m_timer;
};
} // namespace Network
