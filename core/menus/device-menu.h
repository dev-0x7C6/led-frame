#pragma once

#include <core/abstracts/abstract-menu.h>
#include <core/interfaces/iemitter-notify.h>
#include <core/interfaces/ireceiver-notify.h>

#include <list>
#include <map>
#include <set>
#include <memory>

class QAction;

namespace Menu {

	class EmitterMenu;

	class DeviceMenu final
		: public Abstract::AbstractMenu
		, public Interface::IEmitterNotify
		, public Interface::IReceiverNotify

	{
	public:
		explicit DeviceMenu();
		virtual ~DeviceMenu();

		virtual void attached(Interface::IEmitter *emitter) override;
		virtual void attached(Interface::IReceiver *receiver) override;
		virtual void detached(Interface::IEmitter *emitter) override;
		virtual void detached(Interface::IReceiver *receiver) override;

		QAction *beforeAction() const;
		void setBeforeAction(QAction *beforeAction);

	private:
		std::set<Interface::IEmitter *> m_emitters;
		std::list<Interface::IReceiver *> m_receivers;
		std::map<Interface::IReceiver *, QAction *> m_map;
		std::map<Interface::IReceiver *, std::unique_ptr<EmitterMenu>> m_emitterMenu;
		QAction *m_beforeAction;

	};

}
