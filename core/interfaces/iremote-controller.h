#pragma once

#include "core/managers/main-manager.h"

namespace Interface {

class IRemoteController {
public:
	explicit IRemoteController() = default;
	virtual ~IRemoteController() = default;

	virtual bool changeEmitter(int receiverId, int emitterId) = 0;
	virtual bool changeCorrector(int receiverId, int correctorId, double factor, bool enabled) = 0;
};
}
