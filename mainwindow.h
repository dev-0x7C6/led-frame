#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QTimer>

#ifdef Q_OS_UNIX
#include "wiimotedev/deviceevents.h"
#endif

#include "capturethread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  CaptureThread capture;
  QString m_title;

#ifdef Q_OS_UNIX
  WiimotedevDeviceEvents *m_wiimotedevEvents;
  uint64 m_buttons;
#endif

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void setFramerate(int);
  void setBrightness(int value);
  void updateScreenArea(int);
  void about();

  void updateStats(quint32, double, double);

#ifdef Q_OS_UNIX
  void dbusWiimotedevButtons(uint, uint64);
#endif

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
