#pragma once

#include <core/menus/device-menu.h>
#include <core/menus/emitter-configuration-menu.h>
#include "core/interfaces/imulti-notifier.h"

#include <QSystemTrayIcon>
#include <functional>

class QAction;
class QMenu;

namespace Tray {

class SystemTray final : public QSystemTrayIcon, public Interface::IMultiNotifier {
public:
	explicit SystemTray(QObject *parent = nullptr);
	virtual ~SystemTray();

	void setAboutRequestCallback(const std::function<void()> &aboutRequestCallback);
	void setCloseRequestCallback(const std::function<void()> &closeRequestCallback);

protected:
	virtual void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	virtual void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	virtual void attached(Corrector::Interface::ICorrector *corrector) override;
	virtual void detached(Corrector::Interface::ICorrector *corrector) override;
	virtual void modified(Corrector::Interface::ICorrector *corrector) override;

	virtual void attached(Receiver::Interface::IReceiver *receiver) override;
	virtual void detached(Receiver::Interface::IReceiver *receiver) override;
	virtual void modified(Receiver::Interface::IReceiver *receiver) override;

	void setBrightness(Corrector::Interface::ICorrector *corrector);

private:
	std::function<void()> m_aboutRequestCallback;
	std::function<void()> m_closeRequestCallback;

	QAction *m_brightnessAction;
	Menu::DeviceMenu m_deviceMenu;
	Menu::EmitterConfigurationMenu m_emitterConfigurationMenu;
};
}
