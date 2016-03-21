#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QTimer>

namespace Ui {
	class MainWindow;
}

class AmbientDeviceThread;
class ALCScreenEmitter;
class ALCDeviceTreeWidget;
class ALCEmitterManager;
class QMenu;

class MainWindow : public QMainWindow {
	Q_OBJECT
private:
	QSettings *m_settings;
	Ui::MainWindow *ui;
	QList <ALCDeviceTreeWidget *> m_devices;
	ALCEmitterManager *m_screenManager;
	QSystemTrayIcon m_tray;
	QMenu *m_menu;
	QString m_title;

	QAction *m_visible;
	QTimer m_showBrightnessTimer;
	bool m_canClose;

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
	void showColorCorrection(bool);

	void trayActivated(QSystemTrayIcon::ActivationReason reason);
	void trayShowBrightness();
	void trayDrawIcon(double brightness);

};
