#pragma once

#include <QObject>
#include <QSystemTrayIcon>

class QMenu;

namespace Tray {

	class SystemTray final : public QSystemTrayIcon {
		Q_OBJECT
	public:
		explicit SystemTray(QObject *parent = nullptr);
		virtual ~SystemTray() = default;



	protected:
		void redrawTrayIcon(const double &opacity);

	private:
		QMenu *m_menu;

	signals:
		void signalCloseRequest();
		void signalAboutRequest();
	};

}
