#pragma once

#include <core/receivers/interfaces/ireceiver-manager.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <list>

namespace Receiver {
namespace Abstract {

class AbstractReceiverManager : public Receiver::Interface::IReceiverManager {
public:
	explicit AbstractReceiverManager() = default;
	~AbstractReceiverManager() override;

	void attach(Receiver::Interface::IReceiverNotify *notify) override;
	void detach(Receiver::Interface::IReceiverNotify *notify) override;

	Interface::IReceiver *find(const int id) const override;
	const std::list<std::unique_ptr<Receiver::Interface::IReceiver>> &list() const override;

protected:
	void attach(std::unique_ptr<Receiver::Interface::IReceiver> &&receiver) override;
	void detach(Receiver::Interface::IReceiver *receiver) override;

private:
	std::list<std::unique_ptr<Receiver::Interface::IReceiver>> m_receivers;
	std::list<Receiver::Interface::IReceiverNotify *> m_notifiers;
};
}
}
