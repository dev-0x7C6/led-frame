#include "json-protocol.h"

#include <core/interfaces/icorrector.h>
#include <core/interfaces/iemitter.h>
#include <core/interfaces/ireceiver.h>

#include <QJsonDocument>
#include <QJsonObject>

using namespace Network::Protocol;

constexpr auto JSON_PROTOCOL_VERSION = 1;
constexpr auto JSON_DATAGRAM_FIELD = "datagram";

QJsonObject JsonProtocolHelper::header(const ProtocolMessage message, const ProtocolEvent event, const ProtocolSource source) {
	return {
		{"message", name(message)},
		{"version", JSON_PROTOCOL_VERSION},
		{"event", name(event)},
		{"source", name(source)},
	};
}

QString JsonProtocolHelper::notification(const ProtocolEvent event, const ICorrector *corrector) {
	auto json = header(ProtocolMessage::Notification, event, ProtocolSource::Corrector);
	const auto datagram = QJsonObject{
		{"id", corrector->id()},
		{"type", value(corrector->type()).c_str()},
		{"owner", corrector->owner()},
		{"factor", static_cast<int>(corrector->factor().value())},
		{"min", static_cast<int>(corrector->factor().min())},
		{"max", static_cast<int>(corrector->factor().max())},
	};

	json.insert(JSON_DATAGRAM_FIELD, datagram);
	return QJsonDocument(json).toJson();
}

QString JsonProtocolHelper::notification(const ProtocolEvent event, const IEmitter *emitter) {
	auto json = header(ProtocolMessage::Notification, event, ProtocolSource::Emitter);
	const auto datagram = QJsonObject{
		{"id", emitter->id()},
		{"type", QString::fromStdString(value(emitter->type()))},
		{"name", QString::fromStdString(emitter->name())},
		{"description", ""},
	};

	json.insert(JSON_DATAGRAM_FIELD, datagram);
	return QJsonDocument(json).toJson();
}

QString JsonProtocolHelper::notification(const ProtocolEvent event, const IReceiver *receiver) {
	auto json = header(ProtocolMessage::Notification, event, ProtocolSource::Receiver);
	const auto datagram = QJsonObject{
		{"id", receiver->id()},
		{"type", toString(receiver->type())},
		{"name", QString::fromStdString(receiver->name())},
		{"emitter", receiver->connectedEmitterId()},
	};

	json.insert(JSON_DATAGRAM_FIELD, datagram);
	return QJsonDocument(json).toJson();
}
