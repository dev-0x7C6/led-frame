#pragma once

#include <core/correctors/interfaces/icorrector.h>
#include <core/emitters/interfaces/iemitter.h>
#include <core/receivers/interfaces/ireceiver.h>

namespace Network {
namespace Protocol {

enum class ProtocolMessage {
	Notification,
};

enum class ProtocolEvent {
	Attached,
	Detached,
	Modified,
};

enum class ProtocolSource {
	Corrector,
	Emitter,
	Receiver,
};

inline auto name(const ProtocolMessage message) {
	switch (message) {
		case ProtocolMessage::Notification: return "notification";
	}

	return "";
}

inline auto name(const ProtocolEvent event) {
	switch (event) {
		case ProtocolEvent::Attached: return "attached";
		case ProtocolEvent::Detached: return "detached";
		case ProtocolEvent::Modified: return "modified";
	}

	return "";
}

inline auto name(const ProtocolSource source) {
	switch (source) {
		case ProtocolSource::Corrector: return "corrector";
		case ProtocolSource::Emitter: return "emitter";
		case ProtocolSource::Receiver: return "receiver";
	}

	return "";
}

class JsonProtocolHelper final {
public:
	explicit JsonProtocolHelper() = delete;
	virtual ~JsonProtocolHelper() = delete;

	static QJsonObject header(const ProtocolMessage message, const ProtocolEvent event, const ProtocolSource source);
	static QString notification(const ProtocolEvent event, const Corrector::Interface::ICorrector *corrector);
	static QString notification(const ProtocolEvent event, const Emitter::Interface::IEmitter *emitter);
	static QString notification(const ProtocolEvent event, const Receiver::Interface::IReceiver *receiver);
};
}
}
