#pragma once

#include <core/abstracts/abstract-menu.h>
#include <core/interfaces/ireceiver-notify.h>

#include <map>
class QAction;

namespace Menu {

	class DeviceMenu final
		: public Abstract::AbstractMenu
		, public Interface::IReceiverNotify

	{
	public:
		explicit DeviceMenu() = default;
		virtual ~DeviceMenu() = default;

		virtual void attached(Interface::IReceiver *receiver) override;
		virtual void detached(Interface::IReceiver *receiver) override;

	private:
		std::map<Interface::IReceiver *, QAction *> m_map;
	};

}
