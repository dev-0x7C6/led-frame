#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>
#include <QTimer>

#include "wiimotedev/deviceevents.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  QPixmap m_wholeScreen;
  int timerId;
  double m_brightness;

  QPixmap m_screenChunk[4];
  QImage m_imageCache;
  QList< QRgb> m_leds;
  quint32 m_rgbCache[3];

  QTimer m_fpsTimer;

  double m_totalLatency;
  int m_totalCycles;

  QString m_title;

  WiimotedevDeviceEvents *m_wiimotedevEvents;
  uint64 m_buttons;

public:
  enum {
    ScreenFragmentsTop = 0,
    ScreenFragmentsButtom,
    ScreenFragmentsLeft,
    ScreenFragmentsRight
  };

  enum ScreenCaptureAlghoritm {
    ScreenCaptureFullShots = 0,
    ScreenCapturePartialShots,
    ScreenCaptureCriticalShots
  };

explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void timerEvent(QTimerEvent *);

private slots:
  void screenUpdate();
  void setFramerate(int);
  void setBrightness(int value);
  void updateScreenArea(int);
  void calculateFPS();
  void about();

  void dbusWiimotedevButtons(uint, uint64);
  
private:
  Ui::MainWindow *ui;

signals:
  void updateLeds(QList< QRgb> colors);
};

#endif // MAINWINDOW_H
