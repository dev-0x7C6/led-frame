#pragma once

#include <core/menus/device-menu.h>
#include <core/interfaces/ireceiver-notify.h>

#include <QObject>
#include <QSystemTrayIcon>
#include <QMainWindow>

class QMenu;
class QAction;

namespace Tray {

	class SystemTray final
		: public QMainWindow
		, public Interface::IReceiverNotify

	{
		Q_OBJECT
	public:
		explicit SystemTray(QWidget *parent = nullptr);
		virtual ~SystemTray();

		virtual void attached(Interface::IReceiver *receiver) override;
		virtual void detached(Interface::IReceiver *receiver) override;

	protected:
		void redrawTrayIcon(const double &opacity);

	private:
		QSystemTrayIcon m_tray;
		QAction *m_devices;
		Menu::DeviceMenu m_deviceMenu;

	signals:
		void signalCloseRequest();
		void signalAboutRequest();
	};

}
