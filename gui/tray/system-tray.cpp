#include <gui/tray/system-tray.h>
#include <gui/tray/system-tray-icon.h>
#include "core/correctors/interfaces/icorrector.h"

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
	setIcon(SystemTrayIcon::generate(1.0));
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
	auto settings = mainMenu->addAction("Settings");
	auto settingsMenu = new QMenu();
	settings->setMenu(settingsMenu);
	auto settingsEmitters = settingsMenu->addAction("Emitters");
	settings->menu()->insertAction(nullptr, settingsEmitters);
	mainMenu->addSeparator();
	auto about = mainMenu->addAction("About");
	auto quit = mainMenu->addAction("&Quit");

	settingsEmitters->setMenu(m_emitterConfigurationMenu.menu());

	connect(about, &QAction::triggered, [this]() {
		if (m_aboutRequestCallback)
			m_aboutRequestCallback();
	});

	connect(quit, &QAction::triggered, [this]() {
		if (m_closeRequestCallback)
			m_closeRequestCallback();
	});
}

SystemTray::~SystemTray() = default;

void SystemTray::attached(const std::shared_ptr<IEmitter> &emitter) {
	m_deviceMenu.attached(emitter);
	m_emitterConfigurationMenu.attached(emitter);
}

void SystemTray::detached(const std::shared_ptr<IEmitter> &emitter) {
	m_deviceMenu.detached(emitter);
	m_emitterConfigurationMenu.detached(emitter);
}

void SystemTray::modified(const std::shared_ptr<IEmitter> &emitter) {
	m_deviceMenu.modified(emitter);
	m_emitterConfigurationMenu.modified(emitter);
}

void SystemTray::attached(Corrector::Interface::ICorrector *corrector) { setBrightness(corrector); }
void SystemTray::detached(Corrector::Interface::ICorrector *corrector) { setBrightness(corrector); }
void SystemTray::modified(Corrector::Interface::ICorrector *corrector) { setBrightness(corrector); }

void SystemTray::attached(IReceiver *receiver) { m_deviceMenu.attached(receiver); }
void SystemTray::detached(IReceiver *receiver) { m_deviceMenu.detached(receiver); }
void SystemTray::modified(IReceiver *receiver) { m_deviceMenu.modified(receiver); }

void SystemTray::setBrightness(Corrector::Interface::ICorrector *corrector) {
	if (!corrector->isGlobal() || corrector->type() != Enum::CorrectorType::Brightness)
		return;

	const auto brightness = corrector->factor();
	m_brightnessAction->setText(tr("Brightness: %1%").arg(QString::number(std::min(100, static_cast<int>(brightness * 100)))));
	setIcon(SystemTrayIcon::generate(brightness));
}

void SystemTray::setAboutRequestCallback(const std::function<void()> &aboutRequestCallback) {
	m_aboutRequestCallback = aboutRequestCallback;
}

void SystemTray::setCloseRequestCallback(const std::function<void()> &closeRequestCallback) {
	m_closeRequestCallback = closeRequestCallback;
}
