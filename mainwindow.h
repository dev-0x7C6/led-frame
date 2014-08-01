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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QSystemTrayIcon>

#ifdef Q_OS_UNIX
#include "wiimotedev/deviceevents.h"
#endif

namespace Ui {
  class MainWindow;
}

class ALCDeviceThread;
class ALCScreenEmitter;
class ALCDeviceTreeWidget;
class ALCEmitterManager;
class QMenu;

class MainWindow : public QMainWindow {
  Q_OBJECT
private:
  QSettings *m_settings;
  Ui::MainWindow *ui;
  QList <ALCDeviceTreeWidget *> m_devices;
  ALCEmitterManager *m_screenManager;
  QSystemTrayIcon m_tray;
  QMenu *m_menu;
  QString m_title;

  QAction *m_visible;

#ifdef Q_OS_UNIX
  WiimotedevDeviceEvents *m_wiimotedevEvents;
  uint64 m_buttons;
#endif

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  virtual void showEvent(QShowEvent *event);
  virtual void hideEvent(QHideEvent *event);

private:
  void about();
  void showColorCorrection(bool);

  void trayActivated(QSystemTrayIcon::ActivationReason reason);

#ifdef Q_OS_UNIX
  void dbusWiimotedevButtons(uint, uint64);
#endif
};

#endif // MAINWINDOW_H
