#include <core/menus/device-menu.h>
#include <core/menus/emitter-selector-menu.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <QAction>
#include <QMenu>

using namespace Emitter::Interface;
using namespace Receiver::Interface;
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

// TODO: DeviceMenu should reload stuff when emitter is changed
void DeviceMenu::modified(const std::shared_ptr<IEmitter> &emitter) { static_cast<void>(emitter); }

void DeviceMenu::attached(IReceiver *receiver) {
	auto action = std::make_unique<QAction>(QIcon(":/tray.png"), receiver->name(), nullptr);
	menu()->insertAction(m_beforeAction, action.get());
	m_map.insert({receiver, action.get()});
	m_receivers.push_back(receiver);
	m_emitterMenu.emplace(receiver, std::make_unique<EmitterSelectorMenu>(action.get(), receiver));

	for (const auto &emitter : m_emitters)
		m_emitterMenu.at(receiver)->attached(emitter);

	m_actions.emplace_back(std::move(action));
}

void DeviceMenu::detached(IReceiver *receiver) {
	delete m_map.at(receiver);
	m_map.erase(receiver);
	m_receivers.remove(receiver);
	m_emitterMenu.erase(receiver);
}

void DeviceMenu::modified(IReceiver *receiver) {
	m_emitterMenu.at(receiver)->changed();
}

QAction *DeviceMenu::beforeAction() const {
	return m_beforeAction;
}

void DeviceMenu::setBeforeAction(QAction *beforeAction) {
	m_beforeAction = beforeAction;
}

QMenu *DeviceMenu::menu() const {
	return m_menu;
}

void DeviceMenu::setMenu(QMenu *menu) {
	m_menu = menu;
}
