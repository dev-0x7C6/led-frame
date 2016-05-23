#include <core/networking/web-socket.h>
#include <core/interfaces/iemitter.h>

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

void WebSocket::attached(const std::shared_ptr<Interface::IEmitter> &emitter) {
	auto command = emitter->parameters();
	command.insert("command", "emitter_attached");
	auto doc = QJsonDocument(command);
	m_webSocket->sendTextMessage(doc.toJson());
	m_webSocket->flush();
}

void WebSocket::detached(const std::shared_ptr<Interface::IEmitter> &) {
}
