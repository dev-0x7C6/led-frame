#include <core/correctors/interfaces/icorrector.h>
#include <core/emitters/interfaces/iemitter.h>
#include <core/functionals/debug-notification.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <core/types.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace Corrector::Interface;
using namespace Emitter::Interface;
using namespace Functional;
using namespace Receiver::Interface;

namespace {
template <typename type>
std::string mem_addr(type addr) {
	std::stringstream stream;
	stream << "[0x" << std::hex << std::noshowbase << std::setw(2) << std::setfill('0') << reinterpret_cast<u64>(addr) << "] ";
	return stream.str();
}

std::string receiver_dbg(IReceiver *receiver, const std::string &text) {
	std::stringstream stream;
	stream << mem_addr(receiver) << text << " receiver: " << receiver->name().toStdString();
	stream << " (" << (receiver->isEmitterConnected() ? receiver->connectedEmitter()->name().toStdString() : "no connection") << ") ";
	return stream.str();
}

std::string emitter_dbg(IEmitter *emitter, const std::string &text) {
	std::stringstream stream;
	stream << mem_addr(emitter) << text << " emitter: " << emitter->name().toStdString();
	stream << " (ref: " << emitter->usages() << ")";
	return stream.str();
}

//std::string corrector_dbg(ICorrector *corrector, const std::string &text) {
//	std::stringstream stream;
//	stream << mem_addr(corrector) << text << " corrector: " << name(corrector->type()).toStdString();
//	stream << " (enabled: " << corrector->isEnabled() << ", factor: " << corrector->factor().factor() << ")";
//	return stream.str();
//}
}

void DebugNotification::action(const NotifyAction type, const std::shared_ptr<IAtom> &atom) noexcept {
	switch (type) {
		case NotifyAction::Attached:
			std::cout << "[*] attached" << std::endl;
			break;
		case NotifyAction::Detached:
			std::cout << "[*] detached" << std::endl;
			break;
		case NotifyAction::Modified:
			std::cout << "[*] modified" << std::endl;
			break;
	}

	switch (atom->category()) {
		case Category::Corrector:
			std::cout << "corrector" << std::endl;
			break;
		case Category::Emitter:
			std::cout << "emitter" << std::endl;
			break;
		case Category::Receiver:
			std::cout << "receiver" << std::endl;
			break;
		case Category::Undefined:
			std::cout << "undefined" << std::endl;
			break;
	}
}

void DebugNotification::attached(Receiver::Interface::IReceiver *receiver) {
	std::cout << receiver_dbg(receiver, "attached") << std::endl;
}

void DebugNotification::detached(Receiver::Interface::IReceiver *receiver) {
	std::cout << receiver_dbg(receiver, "detached") << std::endl;
}

void DebugNotification::modified(Receiver::Interface::IReceiver *receiver) {
	std::cout << receiver_dbg(receiver, "modified") << std::endl;
}

void DebugNotification::attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	std::cout << emitter_dbg(emitter.get(), "attached") << std::endl;
}

void DebugNotification::detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	std::cout << emitter_dbg(emitter.get(), "detached") << std::endl;
}

void DebugNotification::modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	std::cout << emitter_dbg(emitter.get(), "modified") << std::endl;
}

DebugNotification &DebugNotification::instance() {
	static DebugNotification object;
	return object;
}
