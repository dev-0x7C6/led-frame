#include "emitter-configuration-menu.h"
#include "core/emitters/interfaces/iemitter.h"

#include <QAction>
#include <QMenu>

using namespace Menu;

EmitterConfigurationMenu::EmitterConfigurationMenu()
		: m_menu(std::make_unique<QMenu>()) {
}

EmitterConfigurationMenu::~EmitterConfigurationMenu() = default;

void EmitterConfigurationMenu::attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
	auto action = std::make_unique<QAction>(emitter->name());
	action->setProperty("id", emitter->id());
	m_menu->addAction(action.get());
}

void EmitterConfigurationMenu::detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
}

void EmitterConfigurationMenu::modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) {
}
