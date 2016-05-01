#include <core/menus/emitter-menu.h>
#include <core/interfaces/iemitter.h>

#include <QMenu>
#include <QAction>
#include <QActionGroup>

using namespace Interface;
using namespace Menu;

EmitterMenu::EmitterMenu(QAction *parent, Interface::IReceiver *receiver)
	: m_parent(parent)
	, m_receiver(receiver)
	, m_group(new QActionGroup(nullptr))

{
	setMenu(new QMenu());
	m_parent->setMenu(menu());
	m_actionEmitters = menu()->addAction("Emitters");
	m_actionEmitters->setMenu(new QMenu);
}

void EmitterMenu::attached(const std::shared_ptr<IEmitter> &emitter) {
	auto action = m_actionEmitters->menu()->addAction(emitter->name());
	QObject::connect(action, &QAction::triggered, [this, &emitter](bool) {
		m_receiver->connectEmitter(emitter);
	});

	if (m_receiver->connectedEmitter() == emitter)
		action->setChecked(true);

	action->setCheckable(true);
	m_group->addAction(action);
	m_group->setExclusive(true);
}

void EmitterMenu::detached(const std::shared_ptr<IEmitter> &) {
}
