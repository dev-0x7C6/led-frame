#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QGuiApplication>
//#include <QQuickView>
#include <QScreen>

#include "dialogs/alc-about-dialog.h"
#include "connector/alc-device-manager.h"
#include "connector/alc-device-thread.h"
#include "emitters/plain-color-emitter.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),

  m_settings(new QSettings("AmbientLedDriver", "AmbientLedDriver", this)),
  ui(new Ui::MainWindow),
  m_screenManager(ALCEmitterManager::instance())
  #ifdef Q_OS_UNIX
  ,
    m_wiimotedevEvents(new WiimotedevDeviceEvents),
    m_buttons(0)
  #endif
{
  qRegisterMetaType< QList<QRgb> >("QList< QRgb >");
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
  move (rect.x() + ((rect.width() - width()) / 2), rect.y() + ((rect.height() - height()) / 2) - 50);

#ifdef Q_OS_UNIX
  connect(m_wiimotedevEvents, &WiimotedevDeviceEvents::dbusWiimoteButtons, this, &MainWindow::dbusWiimotedevButtons);
#endif

  connect(ui->actionColor_correction, &QAction::toggled, this, &MainWindow::showColorCorrection);
  connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
  connect(ui->actionAddAnimation, &QAction::triggered, ui->emitters, &ALCEmittersWidget::addAnimationItem);
  connect(ui->actionAddPlainColor, &QAction::triggered, ui->emitters, &ALCEmittersWidget::addPlainColorItem);
  connect(ui->actionAddImageSamples, &QAction::triggered, ui->emitters, &ALCEmittersWidget::addImageItem);
  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->emitters, &ALCEmittersWidget::setup, Qt::QueuedConnection);
  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->screens, &ALCScreensWidget::setup, Qt::QueuedConnection);
}

void MainWindow::about() {
  ALCAboutDialog form(this);
  form.exec();
}

void MainWindow::showColorCorrection(bool visible) {
  ui->colorCorrection->setVisible(visible);
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

MainWindow::~MainWindow()
{
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

  delete ui;
}
