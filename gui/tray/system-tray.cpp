#include <gui/tray/system-tray.h>

#include <QPainter>
#include <QPixmap>
#include <QRadialGradient>
#include <QMenu>

using namespace Tray;

SystemTray::SystemTray(QObject *parent)
	: QObject(parent)
	, m_tray(this)
	, m_menu(new QMenu())

{
	m_tray.setContextMenu(m_menu);
	draw();
	m_menu->addSeparator();
	auto quit = m_menu->addAction("Quit");
	m_tray.show();
	connect(quit, &QAction::triggered, this, &SystemTray::closeRequest);
}

void SystemTray::draw() {
	QPixmap source(":/tray.png");
	QPixmap sheet(22, 22);
	sheet.fill(QColor::fromRgbF(0, 0, 0, 0));
	QPainter painter(&sheet);
	QRadialGradient radialGrad(QPointF(11, 11), 18);
	radialGrad.setColorAt(0, QColor::fromRgb(0x00, 0x95, 0xf8, 0xff));
	radialGrad.setColorAt(0.75, QColor::fromRgb(0x00, 0x95, 0xf8, 0x00));
	painter.setOpacity(1.0);
	painter.setBrush(radialGrad);
	painter.setPen(Qt::NoPen);
	painter.drawRect(0, 0, 22, 22);
	painter.setOpacity(1.0);
	painter.drawPixmap(3, 3, 16, 16, source);
	m_tray.setIcon(QIcon(sheet));
}
