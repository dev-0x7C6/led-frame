#pragma once

#include <core/emitters/interfaces/iemitter-notify.h>

#include <memory>
#include <list>

class QAction;
class QMenu;

namespace Menu {

class EmitterConfigurationMenu final : public Emitter::Interface::IEmitterNotify {
public:
	explicit EmitterConfigurationMenu();
	~EmitterConfigurationMenu() override;

	void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	inline QMenu *menu() { return m_menu.get(); }

private:
	std::unique_ptr<QMenu> m_menu;
	std::list<std::unique_ptr<QAction>> m_actions;
};
}
