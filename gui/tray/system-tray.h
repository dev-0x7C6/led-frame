#pragma once

#include <QObject>
#include <QSystemTrayIcon>

class QMenu;

namespace Tray {

	class SystemTray final : public QObject {
		Q_OBJECT
	public:
		explicit SystemTray(QObject *parent = nullptr);
		virtual ~SystemTray() = default;

		void draw();

	private:
		QSystemTrayIcon m_tray;
		QMenu *m_menu;

	signals:
		void closeRequest();
		void aboutRequest();
	};

}
