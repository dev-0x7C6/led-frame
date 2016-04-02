#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QGuiApplication>
#include <QPainter>
#include <QRadialGradient>
#include <QScreen>
#include <QWheelEvent>

#include "core/devices/device-thread.h"
#include "dialogs/alc-about-dialog.h"
#include "core/devices/device-manager.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_settings(new QSettings("AmbientLedDriver", "AmbientLedDriver", this)),
	ui(new Ui::MainWindow),
	m_tray(this),
	m_menu(new QMenu()),
	m_canClose(false) {
	ui->setupUi(this);
	QRect rect;
	rect = QGuiApplication::primaryScreen()->geometry();
	move(rect.x() + ((rect.width() - width()) / 2), rect.y() + ((rect.height() - height()) / 2) - 50);
	connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::forceClose);
	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
	m_tray.setContextMenu(m_menu);
	m_tray.show();
	m_visible = m_menu->addAction("Visible");
	m_visible->setCheckable(true);
	connect(m_visible, &QAction::triggered, this, &MainWindow::setVisible);
	m_menu->addSeparator();
	connect(m_menu->addAction("Quit"), &QAction::triggered, this, &MainWindow::forceClose);
	connect(&m_tray, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);
	m_settings->beginGroup("MainWindow");
	m_visible->setChecked(m_settings->value("visible", true).toBool());
	m_settings->endGroup();
	m_tray.installEventFilter(this);

	if (!m_visible->isChecked())
		QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);

	trayDrawIcon(1);
	m_showBrightnessTimer.setInterval(250);
	//  connect(&m_showBrightnessTimer, &QTimer::timeout, this, &MainWindow::trayShowBrightness);
	//  InputTest *input = new InputTest;
	//  input->show();
}

void MainWindow::showEvent(QShowEvent *event) {
	Q_UNUSED(event)

	if (isMinimized())
		showNormal();

	m_visible->setChecked(isVisible());
}

void MainWindow::hideEvent(QHideEvent *event) {
	Q_UNUSED(event)

	if (isMinimized())
		hide();

	m_visible->setChecked(isVisible());
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (!m_canClose) {
		event->ignore();
		hide();
	} else
		event->accept();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
	switch (event->type()) {
		case QEvent::Wheel: {
			double brightness = 1.0;
			brightness += (static_cast<QWheelEvent *>(event)->delta() > 0) ? 0.01 : -0.01;
			brightness = qMin(brightness, 2.0);
			brightness = qMax(brightness, 0.0);
			m_showBrightnessTimer.start();
			m_tray.showMessage(QString(), QString());
			trayDrawIcon(brightness);
			return true;
		}

		default:
			break;
	}

	return QObject::eventFilter(object, event);
}

void MainWindow::forceClose() {
	m_canClose = true;
	close();
}

void MainWindow::about() {
	ALCAboutDialog form(this);
	form.exec();
}


void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
		case QSystemTrayIcon::DoubleClick:
			setHidden(isVisible());
			break;

		default:
			break;
	}
}

void MainWindow::trayShowBrightness() {
	m_showBrightnessTimer.stop();
	m_tray.showMessage("Brightness", QString("Current brightness level: %1%").
	                   arg(QString::number(int(100.0))), QSystemTrayIcon::Information, 1000);
}

void MainWindow::trayDrawIcon(double brightness) {
	QPixmap source(":/16x16/leds.png");
	QPixmap sheet(22, 22);
	sheet.fill(QColor::fromRgbF(0, 0, 0, 0));
	QPainter *painter = new QPainter(&sheet);
	QRadialGradient radialGrad(QPointF(11, 11), 22);
	radialGrad.setColorAt(0, QColor::fromRgb(0x00, 0x95, 0xf8, 0xff));
	radialGrad.setColorAt(0.75, QColor::fromRgb(0x00, 0x95, 0xf8, 0x00));
	painter->setOpacity(brightness);
	painter->setBrush(radialGrad);
	painter->setPen(Qt::NoPen);
	painter->drawRect(0, 0, 22, 22);
	painter->setOpacity(1.0);
	painter->drawPixmap(3, 3, 16, 16, source);
	m_tray.setIcon(QIcon(sheet));
	delete painter;
}

MainWindow::~MainWindow() {
	delete ui;
}
