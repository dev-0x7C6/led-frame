#include "mainwindow.h"
#include <QApplication>
#include <QPixmapCache>
#include "classes/alc-color-samples.h"

#include "managers/alc-emitter-manager.h"

const int applicationMajorVersion = 0;
const int applicationMinorVersion = 9;


#include "emitters/image-color-emitter.h"
#include <QImage>

int main(int argc, char *argv[])
{
  QApplication application(argc, argv);
  application.setApplicationName("AmbientLedDriver");
  application.setApplicationVersion(QString("v%1.%2").arg(
                                      QString::number(applicationMajorVersion),
                                      QString::number(applicationMinorVersion)));
  application.setApplicationDisplayName(QString("%1 %2").arg(
                                          application.applicationName(),
                                          application.applicationVersion()));


  ALCEmitterManager::instance();

  MainWindow w;
  w.show();

  
  return application.exec();
}
