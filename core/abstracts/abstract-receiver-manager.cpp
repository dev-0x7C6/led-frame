#include <core/abstracts/abstract-receiver-manager.h>
#include <core/interfaces/ireceiver-notify.h>

using namespace Interface;
using namespace Abstract;

AbstractReceiverManager::~AbstractReceiverManager() {
	m_receivers.clear();
	m_notifiers.clear();
}

void AbstractReceiverManager::attach(IReceiverNotify *notify) {
	m_notifiers.push_back(notify);

	for (const auto &receiver : m_receivers)
		notify->attached(receiver.get());
}

void AbstractReceiverManager::detach(IReceiverNotify *notify) {
	m_notifiers.remove_if([notify](const auto & match) {
		return match == notify;
	});

	for (const auto &receiver : m_receivers)
		notify->detached(receiver.get());
}

void AbstractReceiverManager::attach(std::unique_ptr<IReceiver> &&receiver) {
	auto interface = receiver.get();
	m_receivers.emplace_back(std::move(receiver));

	for (const auto &notify : m_notifiers)
		notify->attached(interface);
}

void AbstractReceiverManager::detach(IReceiver *receiver) {
	m_receivers.remove_if([receiver](const auto & match) {
		return match.get() == receiver;
	});

	for (const auto &notify : m_notifiers)
		notify->detached(receiver);
}
