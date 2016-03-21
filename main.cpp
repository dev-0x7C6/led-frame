#include "mainwindow.h"
#include <QApplication>

#include "managers/alc-emitter-manager.h"
#include "managers/alc-device-manager.h"

const int applicationMajorVersion = 0;
const int applicationMinorVersion = 9;
const int applicationPatchVersion = 5;

int main(int argc, char *argv[]) {
	QApplication application(argc, argv);
	application.setApplicationName("AmbientLedDriver");
	application.setApplicationVersion(QString("v%1.%2.%3").arg(
	                                    QString::number(applicationMajorVersion),
	                                    QString::number(applicationMinorVersion),
	                                    QString::number(applicationPatchVersion)));
	application.setApplicationDisplayName(QString("%1 %2").arg(
	                                        application.applicationName(),
	                                        application.applicationVersion()));
	ALCDeviceManager manager;
	ALCEmitterManager::instance();
	MainWindow window;
	window.show();
	int result = application.exec();
	ALCEmitterManager::instance()->done();
	return result;
}
