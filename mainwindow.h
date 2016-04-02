#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QTimer>

namespace Ui {
	class MainWindow;
}

class DeviceThread;
class QMenu;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	virtual void showEvent(QShowEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	bool eventFilter(QObject *object, QEvent *event);

private:
	void forceClose();
	void about();

	void trayActivated(QSystemTrayIcon::ActivationReason reason);
	void trayShowBrightness();
	void trayDrawIcon(double brightness);

private:
	QSettings *m_settings;
	Ui::MainWindow *ui;
	QSystemTrayIcon m_tray;
	QMenu *m_menu;
	QString m_title;

	QAction *m_visible;
	QTimer m_showBrightnessTimer;
	bool m_canClose;


};
