#include <core/interfaces/icorrector.h>
#include <core/interfaces/iemitter.h>
#include <core/networking/protocols/json-protocol.h>
#include <core/networking/web-socket-connection.h>
#include <core/interfaces/ireceiver.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QWebSocket>

#ifdef QT_DEBUG
#include <iostream>
#endif

using namespace Network::Protocol;
using namespace Emitter::Interface;
using namespace Enum;
using namespace Network;
using namespace Receiver::Interface;

WebSocketConnection::WebSocketConnection(Interface::IRemoteController &remoteController, std::unique_ptr<QWebSocket> &&socket)
		: m_remoteController(remoteController)
		, m_socket(std::move(socket)) {
	QObject::connect(m_socket.get(), &QWebSocket::textMessageReceived, [this](const auto &data) { this->recv(data); });
}

#include <typeindex>

QJsonObject toJson(const std::shared_ptr<IAtom> &atom) {
	//std::cout << static_cast<int>(atom->category()) << std::endl;
	QJsonObject result;
	for (const auto &value : atom->properties()) {
		if (std::type_index(typeid(int)) == std::type_index(value.second.type()))
			result.insert(QString::fromStdString(value.first), std::experimental::any_cast<int>(value.second));
		if (std::type_index(typeid(QString)) == std::type_index(value.second.type()))
			result.insert(QString::fromStdString(value.first), std::experimental::any_cast<QString>(value.second));
		if (std::type_index(typeid(double)) == std::type_index(value.second.type()))
			result.insert(QString::fromStdString(value.first), std::experimental::any_cast<double>(value.second));
		if (std::type_index(typeid(std::string)) == std::type_index(value.second.type()))
			result.insert(QString::fromStdString(value.first), QString::fromStdString(std::experimental::any_cast<std::string>(value.second)));
		if (std::type_index(typeid(const char *)) == std::type_index(value.second.type()))
			result.insert(QString::fromStdString(value.first), std::experimental::any_cast<const char *>(value.second));
	}
	return result;
}

// documentation/protocol/notification.md

void WebSocketConnection::action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept {
	QJsonObject notification{
		{"message", "notification"},
		{"version", 1},
		{"event", toString(type)},
		{"source", toString(atom->category())},
		{"datagram", toJson(atom)}};

	send(QJsonDocument(notification).toJson());
}

WebSocketConnection::~WebSocketConnection() = default;

void WebSocketConnection::send(const QString &message) {
#ifdef QT_DEBUG
	std::cout << "send:\n";
	std::cout << "\t" << message.toStdString() << "\n\n";
#endif
	m_socket->sendTextMessage(message);
	m_socket->flush();
}

void WebSocketConnection::recv(const QString &in) {
	auto json = QJsonDocument::fromJson(in.toUtf8());
	auto obj = json.object();

#ifdef QT_DEBUG
	std::cout << "recv:\n";
	std::cout << "\t" << in.toStdString() << "\n\n";
#endif

	const auto message = obj.value("message").toString();
	const auto event = obj.value("event");

	if (message == "command") {
		if (event == "set_corrector") {
			const auto receiverId = obj.value("receiver").toInt();
			const auto correctorId = obj.value("corrector").toInt();
			const auto factor = obj.value("factor").toInt();
			const auto enabled = obj.value("enabled").toBool();
			m_remoteController.changeCorrector(receiverId, correctorId, factor, enabled);
		}

		if (event == "set_emitter_data") {
			const auto id = obj.value("emitter").toInt();
			const auto data = obj.value("data").toString();
			m_remoteController.changeEmitterData(id, data.toStdString());
		}

		if (event == "set_emitter") {
			const auto receiverId = obj.value("receiver").toInt();
			const auto emitterId = obj.value("emitter").toInt();
			m_remoteController.changeEmitter(receiverId, emitterId);
		}
	}
}
