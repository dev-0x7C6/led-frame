#include "mainwindow.h"
#include <QApplication>

#include <core/containers/application-info-container.h>
#include "managers/alc-emitter-manager.h"
#include "managers/alc-device-manager.h"

int main(int argc, char *argv[]) {
	Container::ApplicationInfoContainer info;
	QApplication application(argc, argv);
	application.setApplicationName(info.applicationName());
	application.setApplicationVersion(info.versionToString());
	application.setApplicationDisplayName(QString("%1 %2").arg(info.applicationName(), info.versionToString()));
	ALCDeviceManager manager;
	ALCEmitterManager::instance();
	MainWindow window;
	window.show();
	int result = application.exec();
	ALCEmitterManager::instance()->done();
	return result;
}
