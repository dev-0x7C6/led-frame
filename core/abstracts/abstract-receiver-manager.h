#pragma once

#include <core/interfaces/ireceiver-manager.h>
#include <core/interfaces/ireceiver.h>

#include <list>

namespace Abstract {

	class AbstractReceiverManager : public Interface::IReceiverManager {
	public:
		explicit AbstractReceiverManager() = default;
		virtual ~AbstractReceiverManager();

		virtual void attach(Interface::IReceiverNotify *notify) override;
		virtual void detach(Interface::IReceiverNotify *notify) override;

	protected:
		virtual void attach(std::unique_ptr<Interface::IReceiver> &&receiver) override;
		virtual void detach(Interface::IReceiver *receiver) override;

	private:
		std::list<std::unique_ptr<Interface::IReceiver>> m_receivers;
		std::list<Interface::IReceiverNotify *> m_notifiers;
	};

}
