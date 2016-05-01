#pragma once

#include <core/interfaces/ireceiver-notify.h>
#include <core/menus/device-menu.h>

#include <QSystemTrayIcon>

class QAction;
class QMenu;

namespace Tray {

	class SystemTray final
		: public QSystemTrayIcon
		, public Interface::IReceiverNotify
		, public Interface::IEmitterNotify

	{
		Q_OBJECT
	public:
		explicit SystemTray(QObject *parent = nullptr);
		virtual ~SystemTray();

		virtual void attached(const std::shared_ptr<Interface::IEmitter> &emitter) override;
		virtual void detached(const std::shared_ptr<Interface::IEmitter> &emitter) override;

		virtual void attached(Interface::IReceiver *receiver) override;
		virtual void detached(Interface::IReceiver *receiver) override;
		virtual void changed(Interface::IReceiver *receiver) override;

		virtual void setBrightness(const float &brightness);

		virtual bool event(QEvent *event) override;

	protected:
		void redrawTrayIcon(const double &opacity);

	private:
		QAction *m_brightnessAction;
		QAction *m_devices;
		Menu::DeviceMenu m_deviceMenu;

	signals:
		void signalWheelChanged(int delta);
		void signalCloseRequest();
		void signalAboutRequest();
	};

}
