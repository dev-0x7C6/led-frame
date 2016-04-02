#include <QApplication>

#include <core/containers/application-info-container.h>
#include <core/devices/device-manager.h>
#include <gui/tray/system-tray.h>

int main(int argc, char *argv[]) {
	Container::ApplicationInfoContainer info;
	QApplication application(argc, argv);
	application.setApplicationName(info.applicationName());
	application.setApplicationVersion(info.versionToString());
	application.setApplicationDisplayName(QString("%1 %2").arg(info.applicationName(), info.versionToString()));
	Device::DeviceManager manager;
	Tray::SystemTray tray;
	QObject::connect(&tray, &Tray::SystemTray::closeRequest, [&application] {
		application.quit();
	});
	return application.exec();
}
