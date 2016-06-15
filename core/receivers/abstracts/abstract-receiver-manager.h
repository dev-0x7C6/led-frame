#pragma once

#include <core/receivers/interfaces/ireceiver-manager.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <list>

namespace Receiver {
namespace Abstract {

class AbstractReceiverManager : public Receiver::Interface::IReceiverManager {
public:
	explicit AbstractReceiverManager() = default;
	virtual ~AbstractReceiverManager();

	virtual void attach(Receiver::Interface::IReceiverNotify *notify) override;
	virtual void detach(Receiver::Interface::IReceiverNotify *notify) override;

	virtual const std::list<std::unique_ptr<Receiver::Interface::IReceiver>> &list() const override;

protected:
	virtual void attach(std::unique_ptr<Receiver::Interface::IReceiver> &&receiver) override;
	virtual void detach(Receiver::Interface::IReceiver *receiver) override;

private:
	std::list<std::unique_ptr<Receiver::Interface::IReceiver>> m_receivers;
	std::list<Receiver::Interface::IReceiverNotify *> m_notifiers;
};
}
}
