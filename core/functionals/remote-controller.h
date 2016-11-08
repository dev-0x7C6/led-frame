#pragma once

#include "core/managers/main-manager.h"

namespace Functional {

class RemoteController final {
public:
	explicit RemoteController(Manager::MainManager &mainManager);
	virtual ~RemoteController() = default;

	bool changeEmitter(int receiverId, int emitterId);
	bool changeCorrector(int receiverId, int correctorId, double factor, bool enabled);

private:
	Manager::MainManager &m_mainManager;
};
}
