#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#ifdef Q_OS_UNIX
#include "wiimotedev/deviceevents.h"
#endif

class ALCDeviceThread;

#include "emitters/screencapturecoloremitter.h"

namespace Ui {
class MainWindow;
}

#include <QComboBox>
#include <QRadioButton>
#include <QTreeWidgetItem>

class ComboBoxItem : public QComboBox
{
    Q_OBJECT

private:
    QTreeWidgetItem *item;
    int column;

public:
    ComboBoxItem(QTreeWidgetItem*, int);

public slots:
    void changeItem(int);

};

class RadioButtonItem  : public QRadioButton
{
    Q_OBJECT

private:
    QTreeWidgetItem *item;
    int column;

public:
    RadioButtonItem (QTreeWidgetItem*, int);

public slots:
    void changeItem(int);

};

class ALCDeviceTreeWidget :public QObject, public QTreeWidgetItem {
  Q_OBJECT
private:
  ALCDeviceThread *m_device;

public:
  ALCDeviceTreeWidget(QTreeWidget *tree, ALCDeviceThread *device)
    :QTreeWidgetItem(tree),
      QObject(0),
      m_device(device)
  {
  }

  ALCDeviceThread * device() { return m_device; }
  void setDevice(ALCDeviceThread *device) { m_device = device; }

private slots:
  void currentIndexChanged(int);

signals:
  void setCustomEmitter(ALCDeviceThread*, int);
};

#include "emitters/animationcoloremitter.h"
#include "connector/alcdevicemanager.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  QSettings *m_settings;
  QList < ColorEmitter*> m_colorEmitters;
  QList < ALCDeviceTreeWidget*> m_devices;

  QString m_title;

  ALCDeviceManager *m_manager;
  AnimationColorEmitter anim;

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
  void deviceConnected(ALCDeviceThread *thread);
  void deviceDisconnected(ALCDeviceThread *thread);

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

private slots:
  void setCustomEmitter(ALCDeviceThread*, int);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
