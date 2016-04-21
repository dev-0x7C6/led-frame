#pragma once

#include <core/menus/device-menu.h>

#include <QObject>
#include <QSystemTrayIcon>

class QMenu;
class QAction;

namespace Tray {

	class SystemTray final : public QSystemTrayIcon {
		Q_OBJECT
	public:
		explicit SystemTray(QObject *parent = nullptr);
		virtual ~SystemTray();


	protected:
		void redrawTrayIcon(const double &opacity);

	private:
		QMenu *m_menu;
		QAction *m_devices;
		Menu::DeviceMenu m_deviceMenu;

	signals:
		void signalCloseRequest();
		void signalAboutRequest();
	};

}
