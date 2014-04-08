#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "connector/alc-device-manager.h"
#include "emitters/animation-color-emitter.h"
#include "emitters/image-color-emitter.h"
#include "emitters/screen-capture-color-emitter.h"
#include "led-configuration-widget.h"
#include "managers/alc-emitter-manager.h"
#include "widgets/alc-devices-widget.h"

#ifdef Q_OS_UNIX
#include "wiimotedev/deviceevents.h"
#endif

namespace Ui { class MainWindow; }

class ALCDeviceThread;
class ScreenCaptureColorEmitter;

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  QSettings *m_settings;
  QList < ALCDeviceTreeWidget*> m_devices;
  ALCEmitterManager *m_screenManager;
  QString m_title;

  double m_statisticAverageFPS;
  double m_statisticAverageLatency;
  double m_statisticAverageThreadUse;
  int m_statisticClock;
  bool m_statisticFirstTime;

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
  void about();

  void setDeviceColorFormat(int);
  void setDeviceIODelay(int);
  void setDeviceBrightness(int);
  void setDeviceBlueColorCorrection(int);
  void setDeviceGreenColorCorrection(int);
  void setDeviceRedColorCorrection(int);

  void showColorCorrection(bool);

  void updateStats(quint32, double, double);

#ifdef Q_OS_UNIX
  void dbusWiimotedevButtons(uint, uint64);
#endif

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
