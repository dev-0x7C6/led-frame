#include <gui/tray/system-tray.h>

#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QRadialGradient>
#include <QWheelEvent>

using namespace Tray;

SystemTray::SystemTray(QWidget *parent)
	: QMainWindow(parent)

{
	auto mainMenu = new QMenu(this);
	auto deviceMenu = new QMenu(this);
	redrawTrayIcon(1.0);
	m_tray.setContextMenu(mainMenu);
	m_tray.show();
	auto actionDevices = mainMenu->addAction("Devices");
	actionDevices->setMenu(deviceMenu);
	m_deviceMenu.setMenu(deviceMenu);
	mainMenu->addSeparator();
	auto about = mainMenu->addAction("About");
	auto quit = mainMenu->addAction("&Quit");
	connect(quit, &QAction::triggered, this, &SystemTray::signalCloseRequest);
	connect(about, &QAction::triggered, this, &SystemTray::signalAboutRequest);
}

SystemTray::~SystemTray() {
}

void SystemTray::attached(Interface::IReceiver *receiver) {
	m_deviceMenu.attached(receiver);
}

void SystemTray::detached(Interface::IReceiver *receiver) {
	m_deviceMenu.detached(receiver);
}

void SystemTray::redrawTrayIcon(const double &opacity) {
	QPixmap source(":/tray.png");
	QPixmap sheet(22, 22);
	sheet.fill(QColor::fromRgbF(0, 0, 0, 0));
	QPainter painter(&sheet);
	QRadialGradient radialGrad(QPointF(11, 11), 18);
	radialGrad.setColorAt(0, QColor::fromRgb(0x00, 0x95, 0xf8, 0xff));
	radialGrad.setColorAt(0.75, QColor::fromRgb(0x00, 0x95, 0xf8, 0x00));
	painter.setOpacity(opacity);
	painter.setBrush(radialGrad);
	painter.setPen(Qt::NoPen);
	painter.drawRect(0, 0, 22, 22);
	painter.setOpacity(1.0);
	painter.drawPixmap(3, 3, 16, 16, source);
	m_tray.setIcon(QIcon(sheet));
}
