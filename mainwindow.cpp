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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>

#include "connector/alc-device-thread.h"
#include "dialogs/alc-about-dialog.h"
#include "emitters/alc-animation-emitter.h"
#include "emitters/alc-color-emitter.h"
#include "emitters/alc-image-emitter.h"
#include "emitters/alc-screen-emitter.h"
#include "managers/alc-device-manager.h"
#include "managers/alc-emitter-manager.h"
#include "widgets/alc-device-widget.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_settings(new QSettings("AmbientLedDriver", "AmbientLedDriver", this)),
  ui(new Ui::MainWindow),
  m_screenManager(ALCEmitterManager::instance()),
  m_tray(QIcon(QPixmap(":/22x22/leds.png").scaled(32, 32)), this),
  m_menu(new QMenu())
#ifdef Q_OS_UNIX
  ,
  m_wiimotedevEvents(new WiimotedevDeviceEvents),
  m_buttons(0)
#endif
{
  qRegisterMetaType<QList<QRgb>>("QList< QRgb >");
  ui->setupUi(this);
  ALCEmitterManager::instance()->addSymulation(ui->qml);
  ui->devices->addSymulation(ui->qml);
  ui->leftWidget->setCurrentIndex(0);
  m_settings->beginGroup("GeneralSettings");
  ui->actionColor_correction->setChecked(m_settings->value("visible", false).toBool());;
  m_settings->endGroup();
  m_settings->beginGroup("WiimotedevSettings");
  ui->wiimoteId->setValue(m_settings->value("wiimoteId", 1).toInt());
  ui->wiimoteBrightness->setChecked(m_settings->value("brightnessControl", true).toBool());
  ui->wiimoteFramerate->setChecked(m_settings->value("framerateControl", true).toBool());
  ui->wiimoteScreen->setChecked(m_settings->value("screenControl", true).toBool());
  m_settings->endGroup();
  QRect rect;
  rect = QGuiApplication::primaryScreen()->geometry();
  move(rect.x() + ((rect.width() - width()) / 2), rect.y() + ((rect.height() - height()) / 2) - 50);
#ifdef Q_OS_UNIX
  connect(m_wiimotedevEvents, &WiimotedevDeviceEvents::dbusWiimoteButtons, this, &MainWindow::dbusWiimotedevButtons);
#endif
  connect(ui->actionColor_correction, &QAction::toggled, this, &MainWindow::showColorCorrection);
  connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
  connect(ui->actionAddAnimation, &QAction::triggered, ui->emitters, &ALCEmitterWidget::addAnimationItem);
  connect(ui->actionAddPlainColor, &QAction::triggered, ui->emitters, &ALCEmitterWidget::addPlainColorItem);
  connect(ui->actionAddImageSamples, &QAction::triggered, ui->emitters, &ALCEmitterWidget::addImageItem);
  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->emitters, &ALCEmitterWidget::setup, Qt::QueuedConnection);
  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->screens, &ALCScreenWidget::setup, Qt::QueuedConnection);
  m_tray.setContextMenu(m_menu);
  m_tray.show();
  m_visible = m_menu->addAction("Visible");
  m_visible->setCheckable(true);
  connect(m_visible, &QAction::triggered, this, &MainWindow::setVisible);
  m_menu->addSeparator();
  m_menu->insertAction(0, ui->actionAddAnimation);
  m_menu->insertAction(0, ui->actionAddImageSamples);
  m_menu->insertAction(0, ui->actionAddPlainColor);
  m_menu->addSeparator();
  m_menu->insertAction(0, ui->actionAddScreenArea);
  m_menu->addSeparator();
  connect(m_menu->addAction("Quit"), &QAction::triggered, this, &MainWindow::close);
  connect(&m_tray, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);

  m_settings->beginGroup("MainWindow");
  m_visible->setChecked(m_settings->value("visible", true).toBool());
  m_settings->endGroup();

  if (!m_visible->isChecked())
    QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
}

void MainWindow::showEvent(QShowEvent *event) {
  m_visible->setChecked(isVisible());
}

void MainWindow::hideEvent(QHideEvent *event) {
  m_visible->setChecked(isVisible());
}

void MainWindow::about() {
  ALCAboutDialog form(this);
  form.exec();
}

void MainWindow::showColorCorrection(bool visible) {
  ui->colorCorrection->setVisible(visible);
}

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
    case QSystemTrayIcon::DoubleClick:
      setHidden(isVisible());
      break;

    default:
      break;
  }
}

#ifdef Q_OS_UNIX
void MainWindow::dbusWiimotedevButtons(uint id, uint64 buttons) {
  if (int(id) != ui->wiimoteId->value())
    return;

  if (ui->wiimoteBrightness->isChecked()) {
    //    if ((buttons & WIIMOTE_BTN_PLUS) && !(m_buttons & WIIMOTE_BTN_PLUS))
    //     ui->brightnessSlider->setValue(ui->brightnessSlider->value() + 4);
    //  if ((buttons & WIIMOTE_BTN_MINUS) && !(m_buttons & WIIMOTE_BTN_MINUS))
    //    ui->brightnessSlider->setValue(ui->brightnessSlider->value() - 4);
  }

  if (ui->wiimoteFramerate->isChecked()) {
    //  if ((buttons & WIIMOTE_BTN_RIGHT) && !(m_buttons & WIIMOTE_BTN_RIGHT))
    //    ui->framerateLimit->setValue(ui->framerateLimit->value() + 2);
    //  if ((buttons & WIIMOTE_BTN_LEFT) && !(m_buttons & WIIMOTE_BTN_LEFT))
    //   ui->framerateLimit->setValue(ui->framerateLimit->value() - 2);
  }

  if (ui->wiimoteScreen->isChecked()) {
    if ((buttons & WIIMOTE_BTN_UP) && !(m_buttons & WIIMOTE_BTN_UP)) {
      //      int idx = ui->screenArea->currentIndex() - 1;
      //      if (idx == -1)
      //        idx = ui->screenArea->count() - 1;
      //      ui->screenArea->setCurrentIndex(idx);
    }

    //    if ((buttons & WIIMOTE_BTN_DOWN) && !(m_buttons & WIIMOTE_BTN_DOWN)) {
    //      int idx = ui->screenArea->currentIndex() + 1;
    //      if (idx == ui->screenArea->count())
    //        idx = 0;
    //      ui->screenArea->setCurrentIndex(idx);
    //    }
  }

  m_buttons = buttons;
}
#endif

MainWindow::~MainWindow() {
  //  m_settings->beginGroup("GeneralSettings");
  //  m_settings->setValue("visible", ui->actionColor_correction->isVisible());
  //  m_settings->setValue("screenId", ui->screenArea->currentIndex());
  //  m_settings->endGroup();
  //  m_settings->beginGroup("LedPreviewSettings");
  //  m_settings->setValue("framerateLimit", ui->ledFramerateLimit->value());
  //  m_settings->setValue("ledGlowSize", ui->ledGlow->value());
  //  m_settings->endGroup();
  m_settings->beginGroup("WiimotedevSettings");
  m_settings->setValue("wiimoteId", ui->wiimoteId->value());
  m_settings->setValue("brightnessControl", ui->wiimoteBrightness->isChecked());
  m_settings->setValue("framerateControl", ui->wiimoteFramerate->isChecked());
  m_settings->setValue("screenControl", ui->wiimoteScreen->isChecked());
  m_settings->endGroup();

  m_settings->beginGroup("MainWindow");
  m_settings->setValue("visible", m_visible->isChecked());
  m_settings->endGroup();
  delete ui;
}
