#include "mainwindow.h"
#include <QApplication>
#include <QPixmapCache>

#include "capturethread.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QPixmapCache::setCacheLimit(1024 * 64);
  MainWindow w;
  w.show();
  
  return a.exec();
}
