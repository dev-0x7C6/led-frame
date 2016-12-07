#include "core/correctors/interfaces/icorrector.h"
#include "core/emitters/interfaces/iemitter.h"
#include "core/networking/web-socket.h"
#include "core/receivers/interfaces/ireceiver.h"
#include "core/networking/protocols/json-protocol.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>
#include <QObject>

#ifdef QT_DEBUG
#include <iostream>
#endif

using namespace Network::Protocol;
using namespace Corrector::Interface;
using namespace Emitter::Interface;
using namespace Enum;
using namespace Network;
using namespace Receiver::Interface;

WebSocketConnection::WebSocketConnection(Interface::IRemoteController &remoteController, std::unique_ptr<QWebSocket> &&socket)
		: m_remoteController(remoteController)
		, m_socket(std::move(socket)) {
	QObject::connect(m_socket.get(), &QWebSocket::textMessageReceived, [this](const auto &data) { this->recv(data); });
}

WebSocketConnection::~WebSocketConnection() = default;

// documentation/protocol/notification.md

void WebSocketConnection::attached(ICorrector *corrector) { send(JsonProtocolHelper::notification(ProtocolEvent::Attached, corrector)); }
void WebSocketConnection::detached(ICorrector *corrector) { send(JsonProtocolHelper::notification(ProtocolEvent::Detached, corrector)); }
void WebSocketConnection::modified(ICorrector *corrector) { send(JsonProtocolHelper::notification(ProtocolEvent::Modified, corrector)); }

void WebSocketConnection::attached(const std::shared_ptr<IEmitter> &emitter) { send(JsonProtocolHelper::notification(ProtocolEvent::Attached, emitter.get())); }
void WebSocketConnection::detached(const std::shared_ptr<IEmitter> &emitter) { send(JsonProtocolHelper::notification(ProtocolEvent::Detached, emitter.get())); }
void WebSocketConnection::modified(const std::shared_ptr<IEmitter> &emitter) { send(JsonProtocolHelper::notification(ProtocolEvent::Modified, emitter.get())); }

void WebSocketConnection::attached(IReceiver *receiver) { send(JsonProtocolHelper::notification(ProtocolEvent::Attached, receiver)); }
void WebSocketConnection::detached(IReceiver *receiver) { send(JsonProtocolHelper::notification(ProtocolEvent::Detached, receiver)); }
void WebSocketConnection::modified(IReceiver *receiver) { send(JsonProtocolHelper::notification(ProtocolEvent::Modified, receiver)); }

void WebSocketConnection::send(const QString &message) {
#ifdef QT_DEBUG
	std::cout << message.toStdString() << std::endl
			  << std::endl;
#endif
	m_socket->sendTextMessage(message);
	m_socket->flush();
}

void WebSocketConnection::recv(const QString &in) {
	auto json = QJsonDocument::fromJson(in.toUtf8());
	auto obj = json.object();

	const auto message = obj.value("message").toString();
	const auto event = obj.value("event");

	if (message == "command") {
		if (event == "set_corrector") {
			const auto receiverId = obj.value("receiver").toInt();
			const auto correctorId = obj.value("corrector").toInt();
			const auto factor = obj.value("factor").toDouble();
			const auto enabled = obj.value("enabled").toBool();
			m_remoteController.changeCorrector(receiverId, correctorId, factor, enabled);
		}

		if (event == "set_emitter") {
			const auto receiverId = obj.value("receiver").toInt();
			const auto emitterId = obj.value("emitter").toInt();
			m_remoteController.changeEmitter(receiverId, emitterId);
		}
	}
}
