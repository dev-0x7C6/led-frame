#pragma once

#include <core/receivers/interfaces/ireceiver-notify.h>
#include <core/menus/device-menu.h>
#include <core/menus/emitter-configuration-menu.h>

#include <QSystemTrayIcon>
#include <functional>

class QAction;
class QMenu;

namespace Tray {

class SystemTray final
	: public QSystemTrayIcon,
	  public Receiver::Interface::IReceiverNotify,
	  public Emitter::Interface::IEmitterNotify {
public:
	explicit SystemTray(QObject *parent = nullptr);
	virtual ~SystemTray();

	virtual void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	virtual void attached(Receiver::Interface::IReceiver *receiver) override;
	virtual void detached(Receiver::Interface::IReceiver *receiver) override;
	virtual void modified(Receiver::Interface::IReceiver *receiver) override;

	virtual void setBrightness(double brightness);

	void setAboutRequestCallback(const std::function<void()> &aboutRequestCallback);
	void setCloseRequestCallback(const std::function<void()> &closeRequestCallback);

private:
	std::function<void()> m_aboutRequestCallback;
	std::function<void()> m_closeRequestCallback;

	QAction *m_brightnessAction;
	Menu::DeviceMenu m_deviceMenu;
	Menu::EmitterConfigurationMenu m_emitterConfigurationMenu;
};
}
