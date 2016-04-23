#include <core/menus/device-menu.h>
#include <core/interfaces/ireceiver.h>

#include <QMenu>
#include <QAction>

using namespace Interface;
using namespace Menu;

void DeviceMenu::attached(IReceiver *receiver) {
	auto action = new QAction(receiver->name(), nullptr);
	menu()->insertAction(0, action);
	m_map.insert({ receiver, action });
}

void DeviceMenu::detached(IReceiver *receiver) {
	delete m_map.at(receiver);
	m_map.erase(receiver);
}
