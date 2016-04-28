#pragma once

#include <core/abstracts/abstract-menu.h>
#include <core/interfaces/iemitter-notify.h>
#include <core/interfaces/ireceiver.h>

class QAction;
class QActionGroup;

namespace Menu {

	class EmitterMenu final
		: public Abstract::AbstractMenu
		, public Interface::IEmitterNotify

	{
	public:
		explicit EmitterMenu(QAction *parent, Interface::IReceiver *receiver);
		virtual ~EmitterMenu() = default;

		virtual void attached(Interface::IEmitter *emitter) override;
		virtual void detached(Interface::IEmitter *emitter) override;

	private:
		QAction *m_parent;
		Interface::IReceiver *m_receiver;
		QActionGroup *m_group;
	};

}
