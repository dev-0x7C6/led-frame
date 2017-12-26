#include <core/interfaces/icorrector.h>
#include <core/interfaces/iemitter.h>
#include <core/networking/protocols/json-protocol.h>
#include <core/networking/web-socket-connection.h>
#include <core/interfaces/ireceiver.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QWebSocket>
#include <QTimer>

#ifdef QT_DEBUG
#include <iostream>
#endif

#include <chrono>
#include <typeindex>

using namespace Network::Protocol;
using namespace Enum;
using namespace Network;

using namespace std::chrono_literals;

WebSocketConnection::WebSocketConnection(Interface::IRemoteController &remoteController, std::unique_ptr<QWebSocket> &&socket)
		: m_remoteController(remoteController)
		, m_socket(std::move(socket)) {
	QObject::connect(m_socket.get(), &QWebSocket::textMessageReceived, [this](const auto &data) { this->recv(data); });
	QObject::connect(&m_timer, &QTimer::timeout, [this]() {
		std::lock_guard<std::mutex> _(m_mutex);
		while (!m_outgoingQueue.empty()) {
			send(m_outgoingQueue.front());
			m_outgoingQueue.pop();
		}
	});
	m_timer.start(5ms);
}

WebSocketConnection::~WebSocketConnection() = default;

template <class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

QJsonObject toJson(const std::shared_ptr<IAtom> &atom) {
	QJsonObject result;

	for (const auto & [ key, value ] : atom->properties()) {
		std::visit(overloaded{
					   [&result, key = key](auto arg) {
						   if constexpr (std::is_same_v<decltype (arg), std::string>) {
							   result.insert(key.c_str(), arg.c_str());
	}
	else {
		result.insert(QString::fromStdString(key), arg);
	};
}
,
},
			value);
}

return result;
}

// documentation/protocol/notification.md

void WebSocketConnection::action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept {
	std::lock_guard<std::mutex> _(m_mutex);
	QJsonObject notification{
		{"message", "notification"},
		{"version", 1},
		{"event", toString(type)},
		{"source", toString(atom->category())},
		{"datagram", toJson(atom)}};

	m_outgoingQueue.push(QJsonDocument(notification).toJson());
}

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
