#include "remote-controller.h"

#include <core/interfaces/ireceiver.h>
#include <core/interfaces/iemitter.h>

using namespace Functional;

RemoteController::RemoteController(Manager::MainManager &mainManager)
		: m_mainManager(mainManager) {
}

bool RemoteController::changeEmitter(int receiverId, int emitterId) {
	auto receiver = std::static_pointer_cast<Receiver::Interface::IReceiver>(m_mainManager.atoms().find(Category::Receiver, receiverId));

	if (!receiver)
		return false;

	auto emitter = std::static_pointer_cast<Emitter::Interface::IEmitter>(m_mainManager.atoms().find(Category::Emitter, emitterId));

	if (!emitter)
		return false;

	receiver->connectEmitter(emitter);
	return true;
}

bool RemoteController::changeEmitterData(int emitterId, const std::string &data) {
	auto emitter = std::static_pointer_cast<Emitter::Interface::IEmitter>(m_mainManager.atoms().find(Category::Emitter, emitterId));
	static_cast<void>(data);

	if (!emitter)
		return false;

	//TODO: Emitter should interpret instruction from data

	return true;
}

bool RemoteController::changeCorrector(int receiverId, int correctorId, correct_t factor, bool enabled) {
	auto receiver = std::static_pointer_cast<Receiver::Interface::IReceiver>(m_mainManager.atoms().find(Category::Receiver, receiverId));

	if (!receiver) {
		if (receiverId == -1) {
			auto corrector = std::static_pointer_cast<ICorrector>(m_mainManager.atoms().find(Category::Corrector, correctorId));

			if (!corrector) return false;

			corrector->setFactor(factor);
			corrector->setEnabled(enabled);
			return true;
		}
	}

	return false;
}
