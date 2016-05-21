#pragma once

#include <memory>

namespace Interface {

class IReceiver;
class IReceiverNotify;

class IReceiverManager {
public:
	explicit IReceiverManager() = default;
	virtual ~IReceiverManager() = default;

	virtual void attach(IReceiverNotify *notify) = 0;
	virtual void detach(IReceiverNotify *notify) = 0;

protected:
	virtual void attach(std::unique_ptr<IReceiver> &&receiver) = 0;
	virtual void detach(IReceiver *receiver) = 0;
};
}
