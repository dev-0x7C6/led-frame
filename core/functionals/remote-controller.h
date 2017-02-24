#pragma once

#include <core/interfaces/iremote-controller.h>
#include <core/managers/main-manager.h>

namespace Functional {

class RemoteController final : public Interface::IRemoteController {
public:
	explicit RemoteController(Manager::MainManager &mainManager);
	~RemoteController() override = default;

	bool changeCorrector(int receiverId, int correctorId, correct_t factor, bool enabled) override;
	bool changeEmitter(int receiverId, int emitterId) override;
	bool changeEmitterData(int emitterId, const std::string &data) override;

private:
	Manager::MainManager &m_mainManager;
};
}
