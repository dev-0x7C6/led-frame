#include <core/interfaces/ireceiver.h>
#include <core/menus/device-menu.h>
#include <core/menus/emitter-menu.h>

#include <QAction>
#include <QMenu>

using namespace Interface;
using namespace Menu;

DeviceMenu::DeviceMenu()
	: m_beforeAction(nullptr) {
}

DeviceMenu::~DeviceMenu() {
}

void DeviceMenu::attached(const std::shared_ptr<IEmitter> &emitter) {
	m_emitters.insert(emitter);

	for (const auto &receiver : m_receivers)
		m_emitterMenu.at(receiver)->attached(emitter);
}

void DeviceMenu::detached(const std::shared_ptr<IEmitter> &emitter) {
	m_emitters.erase(emitter);

	for (const auto &receiver : m_receivers)
		m_emitterMenu.at(receiver)->detached(emitter);
}

void DeviceMenu::attached(IReceiver *receiver) {
	auto action = new QAction(receiver->name(), nullptr);
	menu()->insertAction(m_beforeAction, action);
	m_map.insert({ receiver, action });
	m_receivers.push_back(receiver);
	m_emitterMenu.emplace(receiver, std::make_unique<EmitterMenu>(action, receiver));

	for (const auto &emitter : m_emitters)
		m_emitterMenu.at(receiver)->attached(emitter);
}

void DeviceMenu::detached(IReceiver *receiver) {
	delete m_map.at(receiver);
	m_map.erase(receiver);
	m_receivers.remove(receiver);
	m_emitterMenu.erase(receiver);
}

QAction *DeviceMenu::beforeAction() const {
	return m_beforeAction;
}

void DeviceMenu::setBeforeAction(QAction *beforeAction) {
	m_beforeAction = beforeAction;
}