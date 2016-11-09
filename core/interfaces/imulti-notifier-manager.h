#pragma once

#include "core/interfaces/imulti-notifier.h"

namespace Interface {

class IMutliNotifierManager {
public:
	explicit IMutliNotifierManager() = default;
	virtual ~IMutliNotifierManager() = default;

	virtual void attach(IMultiNotifier &notifier) = 0;
	virtual void detach(IMultiNotifier &notifier) = 0;
};
}
