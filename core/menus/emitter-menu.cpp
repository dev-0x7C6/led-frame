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
	action->setIcon(icon(emitter->type()));
	m_map.insert({emitter.get(), action});
	QObject::connect(action, &QAction::triggered, [this, &emitter](bool) {
		m_receiver->connectEmitter(emitter);
	});
	action->setCheckable(true);

	if (m_receiver->connectedEmitter() == emitter)
		action->setChecked(true);

	m_group->addAction(action);
	m_group->setExclusive(true);
}

void EmitterMenu::changed() {
	QAction *action;

	try {
		action = m_map.at(m_receiver->connectedEmitter().get());
	} catch (const std::out_of_range &) {
		action = nullptr;
	}

	if (action)
		action->setChecked(true);
}

void EmitterMenu::detached(const std::shared_ptr<IEmitter> &) {
}
