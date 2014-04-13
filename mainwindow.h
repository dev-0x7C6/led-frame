#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "connector/alc-device-manager.h"
#include "emitters/animation-color-emitter.h"
#include "emitters/image-color-emitter.h"
#include "emitters/screen-capture-color-emitter.h"
#include "managers/alc-emitter-manager.h"
#include "widgets/alc-devices-widget.h"

#ifdef Q_OS_UNIX
#include "wiimotedev/deviceevents.h"
#endif

namespace Ui { class MainWindow; }

class ALCDeviceThread;
class ScreenCaptureColorEmitter;

class MainWindow : public QMainWindow {
  Q_OBJECT
private:
  QSettings *m_settings;
  Ui::MainWindow *ui;
  QList < ALCDeviceTreeWidget*> m_devices;
  ALCEmitterManager *m_screenManager;
  QString m_title;

#ifdef Q_OS_UNIX
  WiimotedevDeviceEvents *m_wiimotedevEvents;
  uint64 m_buttons;
#endif

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  void about();

  void showColorCorrection(bool);

#ifdef Q_OS_UNIX
  void dbusWiimotedevButtons(uint, uint64);
#endif
};

#endif // MAINWINDOW_H
