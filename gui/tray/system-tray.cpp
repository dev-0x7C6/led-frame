#include <gui/tray/system-tray.h>

#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QRadialGradient>
#include <QWheelEvent>

using namespace Tray;

SystemTray::SystemTray(QObject *parent)
	: QSystemTrayIcon(parent)
	, m_menu(new QMenu())

{
	setContextMenu(m_menu);
	redrawTrayIcon(1.0);
	m_devices = m_menu->addAction("Devices");
	m_devices->setMenu(m_deviceMenu.menu());
	m_menu->addSeparator();
	auto about = m_menu->addAction("About");
	auto quit = m_menu->addAction("&Quit");
	show();
	connect(quit, &QAction::triggered, this, &SystemTray::signalCloseRequest);
	connect(about, &QAction::triggered, this, &SystemTray::signalAboutRequest);
}

SystemTray::~SystemTray() {
	delete m_deviceMenu;
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
	setIcon(QIcon(sheet));
}
