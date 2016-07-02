#include <core/correctors/interfaces/icorrector.h>
#include <core/emitters/interfaces/iemitter.h>
#include <core/networking/web-socket.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>

using namespace Corrector::Interface;
using namespace Emitter::Interface;
using namespace Enum;
using namespace Network;
using namespace Receiver::Interface;

template <typename type>
void WebSocket::send(type *corrector, const QString &command) {
	auto json = corrector->parameters();
	json.insert("command", command);
	auto doc = QJsonDocument(json);
	sendTextMessage(doc.toJson());
}

WebSocket::WebSocket(QWebSocket *socket, QObject *parent)
		: QObject(parent)
		, m_webSocket(socket) {
	connect(m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocket::textMessageReceived);
}

void WebSocket::sendTextMessage(const QString &message) {
	m_webSocket->sendTextMessage(message);
	m_webSocket->flush();
}

void WebSocket::attached(ICorrector *corrector) { send<std::remove_pointer<decltype(corrector)>::type>(corrector, "corrector_attached"); }
void WebSocket::detached(ICorrector *corrector) { send<std::remove_pointer<decltype(corrector)>::type>(corrector, "corrector_detached"); }
void WebSocket::modified(ICorrector *corrector) { send<std::remove_pointer<decltype(corrector)>::type>(corrector, "corrector_modified"); }
void WebSocket::attached(const std::shared_ptr<IEmitter> &emitter) { send<std::remove_pointer<decltype(emitter.get())>::type>(emitter.get(), "emitter_attached"); }
void WebSocket::detached(const std::shared_ptr<IEmitter> &emitter) { send<std::remove_pointer<decltype(emitter.get())>::type>(emitter.get(), "emitter_detached"); }
void WebSocket::modified(const std::shared_ptr<IEmitter> &emitter) { send<std::remove_pointer<decltype(emitter.get())>::type>(emitter.get(), "emitter_modified"); }
void WebSocket::attached(IReceiver *receiver) { send<std::remove_pointer<decltype(receiver)>::type>(receiver, "receiver_attached"); }
void WebSocket::detached(IReceiver *receiver) { send<std::remove_pointer<decltype(receiver)>::type>(receiver, "receiver_detached"); }
void WebSocket::modified(IReceiver *receiver) { send<std::remove_pointer<decltype(receiver)>::type>(receiver, "receiver_modified"); }
