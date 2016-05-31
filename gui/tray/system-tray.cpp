#include <gui/tray/system-tray.h>

#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QRadialGradient>
#include <QWheelEvent>

using namespace Emitter::Interface;
using namespace Receiver::Interface;
using namespace Tray;

SystemTray::SystemTray(QObject *parent)
		: QSystemTrayIcon(parent)

{
	auto mainMenu = new QMenu();
	redrawTrayIcon(1.0);
	setContextMenu(mainMenu);
	show();
	mainMenu->addAction("Devices")->setEnabled(false);
	mainMenu->addSeparator();
	m_deviceMenu.setBeforeAction(mainMenu->addSeparator());
	m_deviceMenu.setMenu(mainMenu);
	mainMenu->addSeparator();
	m_brightnessAction = mainMenu->addAction("");
	m_brightnessAction->setEnabled(false);
	mainMenu->addSeparator();
	auto about = mainMenu->addAction("About");
	auto quit = mainMenu->addAction("&Quit");
	connect(quit, &QAction::triggered, this, &SystemTray::signalCloseRequest);
	connect(about, &QAction::triggered, this, &SystemTray::signalAboutRequest);
}

SystemTray::~SystemTray() {
}

void SystemTray::attached(const std::shared_ptr<IEmitter> &emitter) {
	m_deviceMenu.attached(emitter);
}

void SystemTray::detached(const std::shared_ptr<IEmitter> &emitter) {
	m_deviceMenu.detached(emitter);
}

void SystemTray::attached(Receiver::Interface::IReceiver *receiver) {
	m_deviceMenu.attached(receiver);
}

void SystemTray::detached(Receiver::Interface::IReceiver *receiver) {
	m_deviceMenu.detached(receiver);
}

void SystemTray::modified(IReceiver *receiver) {
	m_deviceMenu.modified(receiver);
}

void SystemTray::setBrightness(const float &brightness) {
	m_brightnessAction->setText(tr("Brightness: %1%").arg(QString::number(std::min(100, static_cast<int>(brightness * 100)))));
	redrawTrayIcon(brightness);
}

bool SystemTray::event(QEvent *event) {
	if (event->type() == QEvent::Wheel)
		emit signalWheelChanged(static_cast<QWheelEvent *>(event)->delta());

	return QSystemTrayIcon::event(event);
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
