#pragma once

#include <core/interfaces/iremote-controller.h>
#include <core/managers/main-manager.h>

namespace Functional {

class RemoteController final : public Interface::IRemoteController {
public:
	explicit RemoteController(Manager::MainManager &mainManager);
	~RemoteController() override = default;

	bool changeEmitter(int receiverId, int emitterId) override;
	bool changeCorrector(int receiverId, int correctorId, correct_t factor, bool enabled) override;

private:
	Manager::MainManager &m_mainManager;
};
}
