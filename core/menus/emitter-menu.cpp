#include <core/menus/emitter-menu.h>
#include <core/interfaces/iemitter.h>

#include <QMenu>
#include <QAction>

using namespace Menu;

EmitterMenu::EmitterMenu(QAction *parent, Interface::IReceiver *receiver)
	: m_parent(parent)
	, m_receiver(receiver) {
	setMenu(new QMenu());
	m_parent->setMenu(menu());
}

void EmitterMenu::attached(Interface::IEmitter *emitter) {
	menu()->addAction(emitter->name());
}

void EmitterMenu::detached(Interface::IEmitter *) {
}
