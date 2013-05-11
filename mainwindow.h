#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QSettings>

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
  QSettings *m_settings;
  CaptureThread capture;
  QString m_title;

  QTreeWidgetItem *m_statisticReference[3];
  double m_statisticAverageFPS[3];
  double m_statisticAverageLatency[3];
  double m_statisticAverageThreadUse[3];
  int m_statisticClock[3];

#ifdef Q_OS_UNIX
  WiimotedevDeviceEvents *m_wiimotedevEvents;
  uint64 m_buttons;
#endif

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void setGlowSize(int);
  void setFramerate(int);
  void setFramerateLed(int);
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
