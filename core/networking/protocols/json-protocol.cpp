#include "json-protocol.h"

#include "core/correctors/interfaces/icorrector.h"
#include "core/emitters/interfaces/iemitter.h"
#include "core/receivers/interfaces/ireceiver.h"

#include <QJsonDocument>

using namespace Network::Protocol;

constexpr auto JSON_PROTOCOL_VERSION = 1;
constexpr auto JSON_DATAGRAM_FIELD = "data";

QJsonObject JsonProtocolHelper::header(const ProtocolMessage message, const ProtocolEvent event, const ProtocolSource source) {
	return {
		{"message", name(message)},
		{"version", JSON_PROTOCOL_VERSION},
		{"event", name(event)},
		{"source", name(source)},
	};
}

QString JsonProtocolHelper::notification(const ProtocolEvent event, const Corrector::Interface::ICorrector *corrector) {
	auto json = header(ProtocolMessage::Notification, event, ProtocolSource::Corrector);
	const QJsonObject datagram{
		{"id", corrector->id()},
		{"type", static_cast<int>(corrector->type())},
		{"owner", corrector->owner()},
		{"factor", corrector->factor()},
		{"min", corrector->minimumFactor()},
		{"max", corrector->maximumFactor()},
	};

	json.insert(JSON_DATAGRAM_FIELD, datagram);
	return QJsonDocument(json).toJson();
}

QString JsonProtocolHelper::notification(const ProtocolEvent event, const Emitter::Interface::IEmitter *emitter) {
	auto json = header(ProtocolMessage::Notification, event, ProtocolSource::Emitter);
	const QJsonObject datagram{
		{"id", emitter->id()},
		{"type", static_cast<int>(emitter->type())},
		{"name", emitter->name()},
		{"description", ""},
	};

	json.insert(JSON_DATAGRAM_FIELD, datagram);
	return QJsonDocument(json).toJson();
}

QString JsonProtocolHelper::notification(const ProtocolEvent event, const Receiver::Interface::IReceiver *receiver) {
	auto json = header(ProtocolMessage::Notification, event, ProtocolSource::Receiver);
	const QJsonObject datagram{
		{"id", receiver->id()},
		{"type", static_cast<int>(receiver->type())},
		{"name", receiver->name()},
		{"emitter", receiver->connectedEmitter()->id()},
	};

	json.insert(JSON_DATAGRAM_FIELD, datagram);
	return QJsonDocument(json).toJson();
}
