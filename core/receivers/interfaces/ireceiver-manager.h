#pragma once

#include <core/interfaces/inotification-callback.h>

#include <memory>
#include <list>

namespace Receiver {
namespace Interface {

class IReceiver;
class IReceiverNotify;

class IReceiverManager : public ::Interface::INotificationCallback {
public:
	explicit IReceiverManager() = default;
	virtual ~IReceiverManager() = default;

	virtual void attach(IReceiverNotify *notify) = 0;
	virtual void detach(IReceiverNotify *notify) = 0;

	virtual const std::list<std::unique_ptr<IReceiver>> &list() const = 0;

protected:
	virtual void attach(std::unique_ptr<IReceiver> &&receiver) = 0;
	virtual void detach(IReceiver *receiver) = 0;
};
}
}
