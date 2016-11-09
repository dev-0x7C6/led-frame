#pragma once

#include "core/interfaces/iremote-controller.h"
#include "core/managers/main-manager.h"

namespace Functional {

class RemoteController final : public Interface::IRemoteController {
public:
	explicit RemoteController(Manager::MainManager &mainManager);
	virtual ~RemoteController() = default;

	virtual bool changeEmitter(int receiverId, int emitterId) override;
	virtual bool changeCorrector(int receiverId, int correctorId, double factor, bool enabled) override;

private:
	Manager::MainManager &m_mainManager;
};
}
