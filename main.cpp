#include <QApplication>

#include <core/containers/application-info-container.h>
#include <core/devices/device-manager.h>
#include <gui/tray/system-tray.h>
#include <gui/wizards/device-setup-wizard.h>
#include <gui/dialogs/about-dialog.h>

#include <QSettings>

int main(int argc, char *argv[]) {
	Container::ApplicationInfoContainer info;
	QApplication application(argc, argv);
	application.setQuitOnLastWindowClosed(false);
	application.setApplicationName(info.applicationName());
	application.setApplicationVersion(info.versionToString());
	application.setApplicationDisplayName(QString("%1 %2").arg(info.applicationName(), info.versionToString()));
	Device::DeviceManager manager;
	QSettings settings(info.applicationName(), info.applicationName());
	manager.setRegisterDeviceCallback([&settings](Interface::IReceiver * receiver, const QString & serialNumber) {
		settings.beginGroup("devices");
		settings.beginGroup(serialNumber);

		if (settings.value("name", "").toString().isEmpty()) {
			Wizard::DeviceSetupWizard wizard(receiver);
			wizard.exec();
			receiver->setName(wizard.field("deviceName").toString());
			settings.setValue("name", wizard.field("deviceName").toString());
		}

		settings.endGroup();
		settings.endGroup();
		settings.sync();
		return true;
	});
	Tray::SystemTray tray;
	QObject::connect(&tray, &Tray::SystemTray::closeRequest, [&application] {
		application.quit();
	});
	QObject::connect(&tray, &Tray::SystemTray::aboutRequest, [] {
		Widget::AboutDialog dialog;
		dialog.exec();
	});
	return application.exec();
}
