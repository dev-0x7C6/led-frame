#pragma once

#include <core/interfaces/inotify.h>

#include <memory>
#include <list>

namespace Receiver {
namespace Interface {

class IReceiver;
class IReceiverNotify;

class IReceiverManager : public ::Interface::INotify {
public:
	explicit IReceiverManager()
			: INotify(-1) {}
	~IReceiverManager() override = default;

	virtual void attach(IReceiverNotify *notify) = 0;
	virtual void detach(IReceiverNotify *notify) = 0;

	virtual Receiver::Interface::IReceiver *find(const int id) const = 0;
	virtual const std::list<std::unique_ptr<IReceiver>> &list() const = 0;

protected:
	virtual void attach(std::unique_ptr<IReceiver> &&receiver) = 0;
	virtual void detach(IReceiver *receiver) = 0;
};
}
}
