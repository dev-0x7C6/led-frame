#include <core/emitters/interfaces/iemitter.h>
#include <core/networking/web-socket.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

using namespace Network;
using namespace Enum;

WebSocket::WebSocket(QWebSocket *socket, QObject *parent)
		: QObject(parent)
		, m_webSocket(socket) {
	connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocket::textMessageReceived);
}

void WebSocket::sendTextMessage(const QString &message) {
	m_webSocket->sendTextMessage(message);
	m_webSocket->flush();
}

// TODO: WebSocket should react when corrector is attached, dettached or modified
void WebSocket::attached(Corrector::Interface::ICorrector *corrector) { static_cast<void>(corrector); }
void WebSocket::detached(Corrector::Interface::ICorrector *corrector) { static_cast<void>(corrector); }
void WebSocket::modified(Corrector::Interface::ICorrector *corrector) { static_cast<void>(corrector); }

void WebSocket::attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	auto command = emitter->parameters();
	command.insert("command", "emitter_attached");
	auto doc = QJsonDocument(command);
	m_webSocket->sendTextMessage(doc.toJson());
	m_webSocket->flush();
}

// TODO: WebSocket should react when emitter is detached or modified
void WebSocket::detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) { static_cast<void>(emitter); }
void WebSocket::modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) { static_cast<void>(emitter); }

// TODO: WebSocket should react when receiver is attached or dettached
void WebSocket::attached(Receiver::Interface::IReceiver *receiver) { static_cast<void>(receiver); }
void WebSocket::detached(Receiver::Interface::IReceiver *receiver) { static_cast<void>(receiver); }
void WebSocket::modified(Receiver::Interface::IReceiver *receiver) {
	auto command = receiver->parameters();
	command.insert("command", "receiver_modified");
	auto doc = QJsonDocument(command);
	m_webSocket->sendTextMessage(doc.toJson());
	m_webSocket->flush();
}
