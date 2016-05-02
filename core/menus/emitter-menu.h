#pragma once

#include <core/abstracts/abstract-menu.h>
#include <core/interfaces/iemitter-notify.h>
#include <core/interfaces/ireceiver.h>

#include <map>

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

		virtual void attached(const std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual void detached(const std::shared_ptr<Interface::IEmitter> &emitter) override;
		void changed();

	private:
		std::map<Interface::IEmitter *, QAction *> m_map;

		QAction *m_parent;
		QAction *m_actionEmitters;
		QAction *m_actionCorrectors;
		Interface::IReceiver *m_receiver;
		QActionGroup *m_emitterActionGroup;
		QActionGroup *m_correctorActionGroup;
	};

}
