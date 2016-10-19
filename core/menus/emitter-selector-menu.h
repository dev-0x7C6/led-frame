#pragma once

#include <core/emitters/interfaces/iemitter-notify.h>
#include <core/receivers/interfaces/ireceiver.h>

#include <map>

class QMenu;
class QAction;
class QActionGroup;

namespace Menu {

class EmitterSelectorMenu final
	: public Emitter::Interface::IEmitterNotify

{
public:
	explicit EmitterSelectorMenu(QAction *parent, Receiver::Interface::IReceiver *receiver);
	virtual ~EmitterSelectorMenu() = default;

	virtual void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	void changed();

private:
	std::map<Emitter::Interface::IEmitter *, QAction *> m_map;
	QAction *m_parent;
	QAction *m_actionEmitters;
	QAction *m_actionCorrectors;
	Receiver::Interface::IReceiver *m_receiver;
	QActionGroup *m_emitterActionGroup;
	QMenu *m_menu;
};
}
