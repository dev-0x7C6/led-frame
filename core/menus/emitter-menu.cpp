#include <core/menus/emitter-menu.h>

#include <QMenu>
#include <QAction>

using namespace Menu;

EmitterMenu::EmitterMenu(QAction *parent, Interface::IReceiver *receiver)
	: m_parent(parent)
	, m_receiver(receiver) {
	setMenu(new QMenu());
	m_parent->setMenu(menu());
}

void EmitterMenu::attached(Interface::IEmitter *) {
	menu()->addAction("cos");
}

void EmitterMenu::detached(Interface::IEmitter *) {
}
