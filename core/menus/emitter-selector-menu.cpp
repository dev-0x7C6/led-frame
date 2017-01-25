#include <core/correctors/concretes/corrector-manager.h>
#include <core/emitters/interfaces/iemitter.h>
#include <core/menus/emitter-selector-menu.h>
#include <core/receivers/interfaces/ireceiver.h>
#include <gui/widgets/corrector-settings-dialog.h>

#include <QMenu>
#include <QAction>
#include <QObject>
#include <QActionGroup>
#include <QDebug>

using namespace Emitter::Interface;
using namespace Receiver::Interface;
using namespace Menu;
using namespace Widget;

EmitterSelectorMenu::EmitterSelectorMenu(QAction *parent, Receiver::Interface::IReceiver *receiver)
		: m_parent(parent)
		, m_receiver(receiver)
		, m_emitterActionGroup(new QActionGroup(nullptr))

{
	m_menu = new QMenu();
	m_parent->setMenu(m_menu);
	m_actionEmitters = m_menu->addAction("Emitters");
	m_actionCorrectors = m_menu->addAction("Correctors");
	m_actionEmitters->setMenu(new QMenu);
	m_actionCorrectors->setMenu(new QMenu);

	receiver->correctorManager().enumerate([this](const auto &corrector) {
		if (corrector->owner() == -1)
			return;

		auto action = m_actionCorrectors->menu()->addAction(name(corrector->type()));
		auto menu = new QMenu;
		auto toggle = menu->addAction("Active");
		auto configure = menu->addAction("Configure");
		action->setMenu(menu);
		toggle->setCheckable(true);
		toggle->setChecked(corrector->isEnabled());
		QObject::connect(toggle, &QAction::triggered, [&corrector, toggle](bool checked) {
			corrector->setEnabled(checked);
			toggle->setText(checked ? "Active" : "Inactive");
		});

		QObject::connect(configure, &QAction::triggered, [&corrector](bool) {
			CorrectorSettingsDialog dialog(corrector->factor().factor(), nullptr);
			dialog.setWindowTitle("Corrector: " + name(corrector->type()) + "");
			QObject::connect(&dialog, &CorrectorSettingsDialog::valueChanged, [&corrector](int value) {
				corrector->setFactor(CorrectorSettingsDialog::factor(value) * corrector->factor().max());
			});
			dialog.exec();
		});
	});
}

void EmitterSelectorMenu::attached(const std::shared_ptr<IEmitter> &emitter) {
	auto action = m_actionEmitters->menu()->addAction(emitter->name());
	action->setIcon(icon(emitter->type()));
	m_map.insert({emitter.get(), action});
	QObject::connect(action, &QAction::triggered, [this, &emitter](bool) {
		m_receiver->connectEmitter(emitter);
	});
	action->setCheckable(true);

	if (m_receiver->connectedEmitter() == emitter)
		action->setChecked(true);

	m_emitterActionGroup->addAction(action);
	m_emitterActionGroup->setExclusive(true);
}

// TODO: EmitterMenu should reload stuff when emitter is detached or modiffied
void EmitterSelectorMenu::detached(const std::shared_ptr<IEmitter> &emitter) { static_cast<void>(emitter); }
void EmitterSelectorMenu::modified(const std::shared_ptr<IEmitter> &emitter) { static_cast<void>(emitter); }

void EmitterSelectorMenu::changed() {
	QAction *action;

	try {
		action = m_map.at(m_receiver->connectedEmitter().get());
	} catch (const std::out_of_range &) {
		action = nullptr;
	}

	if (action)
		action->setChecked(true);
}
