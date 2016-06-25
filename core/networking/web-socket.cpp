#include <core/networking/web-socket.h>
#include <core/emitters/interfaces/iemitter.h>

#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>

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
