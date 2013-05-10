#include "mainwindow.h"
#include <QApplication>
#include <QPixmapCache>

#include "capturethread.h"

const int applicationMajorVersion = 0;
const int applicationMinorVersion = 4;

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
  QPixmapCache::setCacheLimit(1024 * 64);
  MainWindow w;
  w.show();
  
  return application.exec();
}
