#include "core/correctors/interfaces/icorrector.h"
#include "core/emitters/interfaces/iemitter.h"
#include "core/networking/web-socket.h"
#include "core/receivers/interfaces/ireceiver.h"
#include "core/networking/protocols/json-protocol.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

#ifdef QT_DEBUG
#include <iostream>
#endif

using namespace Network::Protocol;
using namespace Corrector::Interface;
using namespace Emitter::Interface;
using namespace Enum;
using namespace Network;
using namespace Receiver::Interface;

WebSocket::WebSocket(QWebSocket *socket, QObject *parent)
		: QObject(parent)
		, m_webSocket(socket) {
	connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocket::textMessageReceived);
}

void WebSocket::send(const QString &message) {
#ifdef QT_DEBUG
	std::cout << message.toStdString() << std::endl
			  << std::endl;
#endif
	m_webSocket->sendTextMessage(message);
	m_webSocket->flush();
}

// documentation/protocol/notification

void WebSocket::attached(ICorrector *corrector) { send(JsonProtocolHelper::notification(ProtocolEvent::Attached, corrector)); }
void WebSocket::detached(ICorrector *corrector) { send(JsonProtocolHelper::notification(ProtocolEvent::Detached, corrector)); }
void WebSocket::modified(ICorrector *corrector) { send(JsonProtocolHelper::notification(ProtocolEvent::Modified, corrector)); }

void WebSocket::attached(const std::shared_ptr<IEmitter> &emitter) { send(JsonProtocolHelper::notification(ProtocolEvent::Attached, emitter.get())); }
void WebSocket::detached(const std::shared_ptr<IEmitter> &emitter) { send(JsonProtocolHelper::notification(ProtocolEvent::Detached, emitter.get())); }
void WebSocket::modified(const std::shared_ptr<IEmitter> &emitter) { send(JsonProtocolHelper::notification(ProtocolEvent::Modified, emitter.get())); }

void WebSocket::attached(IReceiver *receiver) { send(JsonProtocolHelper::notification(ProtocolEvent::Attached, receiver)); }
void WebSocket::detached(IReceiver *receiver) { send(JsonProtocolHelper::notification(ProtocolEvent::Detached, receiver)); }
void WebSocket::modified(IReceiver *receiver) { send(JsonProtocolHelper::notification(ProtocolEvent::Modified, receiver)); }
