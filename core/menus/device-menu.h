#pragma once

#include <core/emitters/interfaces/iemitter-notify.h>
#include <core/receivers/interfaces/ireceiver-notify.h>

#include <list>
#include <map>
#include <set>
#include <memory>

class QMenu;
class QAction;

namespace Menu {

class EmitterSelectorMenu;

class DeviceMenu final
	: public Emitter::Interface::IEmitterNotify,
	  public Receiver::Interface::IReceiverNotify

{
public:
	explicit DeviceMenu();
	~DeviceMenu() override;

	void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	void attached(Receiver::Interface::IReceiver *receiver) override;
	void detached(Receiver::Interface::IReceiver *receiver) override;
	void modified(Receiver::Interface::IReceiver *receiver) override;

	QAction *beforeAction() const;
	void setBeforeAction(QAction *beforeAction);

	QMenu *menu() const;
	void setMenu(QMenu *menu);

private:
	std::set<std::shared_ptr<Emitter::Interface::IEmitter>> m_emitters;
	std::list<Receiver::Interface::IReceiver *> m_receivers;
	std::map<Receiver::Interface::IReceiver *, QAction *> m_map;
	std::map<Receiver::Interface::IReceiver *, std::unique_ptr<EmitterSelectorMenu>> m_emitterMenu;
	std::list<std::unique_ptr<QAction>> m_actions;
	QAction *m_beforeAction;
	QMenu *m_menu;
};
}
