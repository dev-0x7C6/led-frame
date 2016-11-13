#include "remote-controller.h"

using namespace Functional;

RemoteController::RemoteController(Manager::MainManager &mainManager)
		: m_mainManager(mainManager) {
}

bool RemoteController::changeEmitter(int receiverId, int emitterId) {
	auto receiver = m_mainManager.receivers().find(receiverId);

	if (!receiver)
		return false;

	auto emitter = m_mainManager.emitters().find(emitterId);

	if (!emitter)
		return false;

	receiver->connectEmitter(emitter);
	return true;
}

bool RemoteController::changeCorrector(int receiverId, int correctorId, double factor, bool enabled) {
	auto receiver = m_mainManager.receivers().find(receiverId);

	if (!receiver) {

		if (receiverId == -1) {
			auto corrector = m_mainManager.correctors().find(correctorId);

			if (!corrector)
				return false;

			corrector->setFactor(factor);
			corrector->setEnabled(enabled);
			return true;
		}

		return false;
	}

	auto corrector = receiver->correctorManager().find(correctorId);

	if (!corrector)
		return false;

	corrector->setFactor(factor);
	corrector->setEnabled(enabled);
	return true;
}
