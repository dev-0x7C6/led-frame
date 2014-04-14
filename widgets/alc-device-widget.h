/**********************************************************************************
 * AmbientLedDriver - https://gitorious.org/ambientleddriver -                    *
 * Copyright (C) 2014  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

#ifndef ALCDEVICEWIDGET_H
#define ALCDEVICEWIDGET_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QObject>
#include <QComboBox>

namespace Ui { class ALCDeviceWidget; }

class ALCDeviceThread;
class ALCEmitter;
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
    :QObject(0),
     QTreeWidgetItem(tree)
  {
    m_device = device;
  }

  ALCDeviceThread * device() { return m_device; }
  void setDevice(ALCDeviceThread *device) { m_device = device; }
  void currentIndexChanged(int);

signals:
  void setCustomEmitter(ALCDeviceThread*, int);
  void setEmitter(ALCDeviceThread*, ALCEmitter*);
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


class ALCSymulationWidget;

class ALCDeviceWidget : public QMainWindow {
  Q_OBJECT
private:
  Ui::ALCDeviceWidget *ui;
  QList < ALCDeviceTreeWidget*> m_devices;
  ALCDeviceManager *m_manager;
  ALCSymulationWidget *m_symulation;

public:
  explicit ALCDeviceWidget(QWidget *parent = 0);
  ~ALCDeviceWidget();

  void addSymulation(ALCSymulationWidget *);

private:
  void deviceConnected(ALCDeviceThread *);
  void deviceDisconnected(ALCDeviceThread *);
  void addWorkspace(ALCDeviceTreeWidget *, ALCDeviceThread *);
  void populate();
  void setEmitter(ALCDeviceThread *device, ALCEmitter *emitter);

  void configureEmitter();

};

#endif // ALCDEVICEWIDGET_H
