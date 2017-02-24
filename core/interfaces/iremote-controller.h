#pragma once

#include <core/managers/main-manager.h>
#include <core/types.h>

namespace Interface {

class IRemoteController {
public:
	explicit IRemoteController() = default;
	virtual ~IRemoteController() = default;

	virtual bool changeCorrector(int receiverId, int correctorId, correct_t factor, bool enabled) = 0;
	virtual bool changeEmitterData(int emitterId, const std::string &data) = 0;
	virtual bool changeEmitter(int receiverId, int emitterId) = 0;
};
}
