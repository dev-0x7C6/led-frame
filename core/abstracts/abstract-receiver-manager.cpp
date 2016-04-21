#include <core/abstracts/abstract-receiver-manager.h>

using namespace Abstract;

AbstractReceiverManager::~AbstractReceiverManager() {
	m_receivers.clear();
	//  m_notifiers.clear()
}

void AbstractReceiverManager::attach(Interface::IReceiverNotify *notify) {
	m_notifiers.push_back(notify);
}

void AbstractReceiverManager::detach(Interface::IReceiverNotify *notify) {
	m_notifiers.remove_if([notify](const auto & match) {
		return match == notify;
	});
}

void AbstractReceiverManager::attach(std::unique_ptr<Interface::IReceiver> &&receiver) {
	m_receivers.emplace_back(std::move(receiver));
}

void AbstractReceiverManager::detach(Interface::IReceiver *receiver) {
	m_receivers.remove_if([receiver](const auto & match) {
		return match.get() == receiver;
	});
}
