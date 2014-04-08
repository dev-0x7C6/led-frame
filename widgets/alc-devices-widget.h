#ifndef ALCDEVICESWIDGET_H
#define ALCDEVICESWIDGET_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QObject>
#include <QComboBox>

namespace Ui { class ALCDevicesWidget; }

class ALCDeviceThread;
class ColorEmitter;
class ALCDeviceManager;

class ComboBoxItem : public QComboBox {
  Q_OBJECT
private:
  QTreeWidgetItem *item;
  int column;

public:
  ComboBoxItem(QTreeWidgetItem*, int);

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

  void currentIndexChanged(int);

signals:
  void setCustomEmitter(ALCDeviceThread*, int);
  void setEmitter(ALCDeviceThread*, ColorEmitter*);
};

#include <QCommandLinkButton>

class DeviceLinkButton :public QCommandLinkButton {
private:
  ALCDeviceThread *m_device;

public:
  void setDeviceThread(ALCDeviceThread *thread) {
    m_device = thread;
  }

  ALCDeviceThread *deviceThread() {
    return m_device;
  }
};

class ALCDevicesWidget : public QMainWindow {
  Q_OBJECT
private:
  Ui::ALCDevicesWidget *ui;
  QList < ALCDeviceTreeWidget*> m_devices;
  ALCDeviceManager *m_manager;

public:
  explicit ALCDevicesWidget(QWidget *parent = 0);
  ~ALCDevicesWidget();

private:
  void deviceConnected(ALCDeviceThread *);
  void deviceDisconnected(ALCDeviceThread *);
  void populate();
  void setEmitter(ALCDeviceThread *device, ColorEmitter *emitter);

  void configureEmitter();

};

#endif // ALCDEVICESWIDGET_H
