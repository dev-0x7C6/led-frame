#pragma once

#include <core/menus/device-menu.h>
#include <core/menus/emitter-configuration-menu.h>
#include <core/interfaces/imulti-notifier.h>

#include <QSystemTrayIcon>
#include <functional>
#include <memory>

class QAction;
class QMenu;

namespace Tray {

class SystemTray final : public QSystemTrayIcon, public Interface::IMultiNotifier {
public:
	explicit SystemTray(QObject *parent = nullptr);
	~SystemTray() override;

	void setAboutRequestCallback(const std::function<void()> &aboutRequestCallback);
	void setCloseRequestCallback(const std::function<void()> &closeRequestCallback);

protected:
	void attached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	void detached(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;
	void modified(const std::shared_ptr<Emitter::Interface::IEmitter> &emitter) override;

	void attached(Corrector::Interface::ICorrector *corrector) override;
	void detached(Corrector::Interface::ICorrector *corrector) override;
	void modified(Corrector::Interface::ICorrector *corrector) override;

	void attached(Receiver::Interface::IReceiver *receiver) override;
	void detached(Receiver::Interface::IReceiver *receiver) override;
	void modified(Receiver::Interface::IReceiver *receiver) override;

	void setBrightness(Corrector::Interface::ICorrector *corrector);

private:
	std::function<void()> m_aboutRequestCallback;
	std::function<void()> m_closeRequestCallback;
	std::unique_ptr<QMenu> m_mainMenu;

	QAction *m_brightnessAction;
	Menu::DeviceMenu m_deviceMenu;
	Menu::EmitterConfigurationMenu m_emitterConfigurationMenu;
};
}
