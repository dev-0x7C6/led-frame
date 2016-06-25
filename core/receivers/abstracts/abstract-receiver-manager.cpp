#include <core/receivers/abstracts/abstract-receiver-manager.h>
#include <core/receivers/interfaces/ireceiver-notify.h>

using namespace Receiver::Abstract;
using namespace Receiver::Interface;

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
	for (const auto &receiver : m_receivers)
		notify->detached(receiver.get());

	m_notifiers.remove_if([notify](const auto &match) {
		return match == notify;
	});
}

const std::list<std::unique_ptr<IReceiver>> &AbstractReceiverManager::list() const {
	return m_receivers;
}

void AbstractReceiverManager::attach(std::unique_ptr<IReceiver> &&receiver) {
	auto interface = receiver.get();

	connect(interface, &INotificationCallback::notify, [this, interface]() {
		for (const auto &notify : m_notifiers)
			notify->modified(interface);

		emit notify();
	});

	m_receivers.emplace_back(std::move(receiver));

	for (const auto &notify : m_notifiers)
		notify->attached(interface);
}

void AbstractReceiverManager::detach(IReceiver *receiver) {
	for (const auto &notify : m_notifiers)
		notify->detached(receiver);

	m_receivers.remove_if([receiver](const auto &match) {
		return match.get() == receiver;
	});
}
