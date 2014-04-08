#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QGuiApplication>
#include <QQuickView>
#include <QScreen>

#include "about.h"
#include "connector/alc-device-manager.h"
#include "connector/alc-device-thread.h"
#include "emitters/plain-color-emitter.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_settings(new QSettings("AmbientLedDriver", "AmbientLedDriver", this)),
#ifdef Q_OS_UNIX
  m_wiimotedevEvents(new WiimotedevDeviceEvents),
  m_buttons(0),
#endif
  ui(new Ui::MainWindow),
  m_screenManager(ALCEmitterManager::instance())
{
  qRegisterMetaType< QList<QRgb> >("QList< QRgb >");
  ui->setupUi(this);

  QRect rect;

  m_statisticAverageFPS = 0;
  m_statisticAverageLatency = 0;
  m_statisticAverageThreadUse = 0;
  m_statisticClock = 0;
  m_statisticFirstTime = true;

//  QQuickView *view = new QQuickView();
//  QWidget *container = QWidget::createWindowContainer(view, this);
//  container->setMinimumSize(500, 400);
//  container->setMaximumSize(500, 400);
//  container->setFocusPolicy(Qt::TabFocus);
//  view->setSource(QUrl("qrc:/qml/main.qml"));
//  ui->qml->addWidget(container);
  ui->qml->setMinimumSize(450,350);

  ui->screenArea->addItem(QIcon(":/16x16/selected-screen.png"), "Not definited");

  rect = QApplication::desktop()->geometry();

  for (register int i = 0; i < 2; ++i) {
    ui->screenArea->addItem(QIcon(":/16x16/all-screens.png"),
      QString("Visible area, x:%1, y:%2 (%3x%4)").arg(
      QString::number(rect.x()),
      QString::number(rect.y()),
      QString::number(rect.width()),
      QString::number(rect.height())),
    -1);
  }

  for (register int i = 0; i < QApplication::desktop()->screenCount(); ++i) {
    rect = QApplication::desktop()->screenGeometry(i);
    ui->screenArea->addItem(QIcon(":/16x16/selected-screen.png"),
      QString("Screen %1: x:%2, y:%3 (%4x%5)").arg(
        QString::number(i),
        QString::number(rect.x()),
        QString::number(rect.y()),
        QString::number(rect.width()),
        QString::number(rect.height())),
      i);
  }

  for (register int i = 0; i < 4; ++i)
    ui->screenArea->addItem(QIcon(":/22x22/animation.png"),
      QString("Animation color profile #%1").arg(QString::number(i+1)), i);

  for (register int i = 0; i < 4; ++i)
    ui->screenArea->addItem(QIcon(":/22x22/animation.png"),
      QString("Plain color profile #%1").arg(QString::number(i+1)), i);

  ui->leftWidget->setCurrentIndex(0);

  connect(ui->actionColor_correction, &QAction::toggled, this, &MainWindow::showColorCorrection);
  connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(ui->ledGlow, SIGNAL(valueChanged(int)), this, SLOT(setGlowSize(int)));
  connect(ui->ledFramerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerateLed(int)));

  m_settings->beginGroup("GeneralSettings");
  ui->actionColor_correction->setChecked(m_settings->value("visible", false).toBool());;
  m_settings->endGroup();


  m_settings->beginGroup("LedPreviewSettings");
  ui->ledFramerateLimit->setValue(m_settings->value("framerateLimit", 24).toInt());
  ui->ledGlow->setValue(m_settings->value("ledGlowSize", 96).toInt());
  m_settings->endGroup();

  m_settings->beginGroup("WiimotedevSettings");
  ui->wiimoteId->setValue(m_settings->value("wiimoteId", 1).toInt());
  ui->wiimoteBrightness->setChecked(m_settings->value("brightnessControl", true).toBool());
  ui->wiimoteFramerate->setChecked(m_settings->value("framerateControl", true).toBool());
  ui->wiimoteScreen->setChecked(m_settings->value("screenControl", true).toBool());
  m_settings->endGroup();

  rect = QGuiApplication::primaryScreen()->geometry();
  move (rect.x() + ((rect.width() - width()) / 2), rect.y() + ((rect.height() - height()) / 2) - 50);

#ifdef Q_OS_UNIX
  connect(m_wiimotedevEvents, SIGNAL(dbusWiimoteButtons(uint,uint64)), this, SLOT(dbusWiimotedevButtons(uint, uint64)));
#endif

  connect(ui->actionAddAnimation, &QAction::triggered, ui->emitters, &ALCEmittersWidget::addAnimationItem);
  connect(ui->actionAddPlainColor, &QAction::triggered, ui->emitters, &ALCEmittersWidget::addPlainColorItem);
  connect(ui->actionAddImageSamples, &QAction::triggered, ui->emitters, &ALCEmittersWidget::addImageItem);

  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->emitters, &ALCEmittersWidget::setup, Qt::QueuedConnection);
  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->screens, &ALCScreensWidget::setup, Qt::QueuedConnection);
  //connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->selections, &ALCSelectionsWidget::setup, Qt::QueuedConnection);

  connect(ui->screenArea, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenArea(int)));

}

void MainWindow::setGlowSize(int value) {
  ui->ledGlowLabel->setText(QString::number(value) + "px");
}

void MainWindow::setFramerateLed(int value) {
  ui->ledFps->setText(QString::number(value) + "fps");
}

void MainWindow::setFramerate(int value) {
  //ui->fpsCount->setText(QString::number(value) + "fps");
  ////////////////////////////////////////capture.setFramerateLimit(value);
}

void MainWindow::updateStats(quint32 fps, double latency, double usage) {
  m_statisticAverageFPS += fps;
  m_statisticAverageLatency += latency;
  m_statisticAverageThreadUse += usage;
  m_statisticClock++;

  if (m_statisticClock == 11 || m_statisticFirstTime) {
    m_statisticAverageFPS /= double(m_statisticClock);
    m_statisticAverageLatency /= double(m_statisticClock);
    m_statisticAverageThreadUse /= double(m_statisticClock);
    m_statisticClock = 1;
    setWindowTitle(m_title +
          QString("Performance: [fps: %1, latency: %2ms, thread usage: %3%]").arg(
            QString::number(fps),
            QString::number(latency, 'f', 1),
            QString::number(usage, 'f', 1)));
    m_statisticFirstTime = false;
  }
}

void MainWindow::about() {
  About form(this);
  form.exec();
}

void MainWindow::setDeviceColorFormat(int value) { }
void MainWindow::setDeviceIODelay(int value) { }

void MainWindow::setDeviceBrightness(int value) {
  for (register int i = 0; i < m_devices.count(); ++i)
    m_devices[i]->device()->setBrightness(value / 100.0);
}

void MainWindow::setDeviceBlueColorCorrection(int value) {
  for (register int i = 0; i < m_devices.count(); ++i)
    m_devices[i]->device()->setBlueCorrection(value / 100.0);
}

void MainWindow::setDeviceGreenColorCorrection(int value) {
  for (register int i = 0; i < m_devices.count(); ++i)
    m_devices[i]->device()->setGreenCorrection(value / 100.0);
}

void MainWindow::setDeviceRedColorCorrection(int value) {
  for (register int i = 0; i < m_devices.count(); ++i)
    m_devices[i]->device()->setRedCorrection(value / 100.0);
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
      int idx = ui->screenArea->currentIndex() - 1;
      if (idx == -1)
        idx = ui->screenArea->count() - 1;
      ui->screenArea->setCurrentIndex(idx);
    }
    if ((buttons & WIIMOTE_BTN_DOWN) && !(m_buttons & WIIMOTE_BTN_DOWN)) {
      int idx = ui->screenArea->currentIndex() + 1;
      if (idx == ui->screenArea->count())
        idx = 0;
      ui->screenArea->setCurrentIndex(idx);
    }
  }

  m_buttons = buttons;
}

#endif

void ALCDeviceTreeWidget::currentIndexChanged(int idx) {
  Q_UNUSED(idx)
  ComboBoxItem *cmb = dynamic_cast < ComboBoxItem*> ( sender());
  emit setEmitter(m_device, reinterpret_cast< ColorEmitter*> (qvariant_cast < void*> (cmb->currentData())));
}

MainWindow::~MainWindow()
{
  m_settings->beginGroup("GeneralSettings");
  m_settings->setValue("visible", ui->actionColor_correction->isVisible());
  m_settings->setValue("screenId", ui->screenArea->currentIndex());
  m_settings->endGroup();

  m_settings->beginGroup("LedPreviewSettings");
  m_settings->setValue("framerateLimit", ui->ledFramerateLimit->value());
  m_settings->setValue("ledGlowSize", ui->ledGlow->value());
  m_settings->endGroup();

  m_settings->beginGroup("WiimotedevSettings");
  m_settings->setValue("wiimoteId", ui->wiimoteId->value());
  m_settings->setValue("brightnessControl", ui->wiimoteBrightness->isChecked());
  m_settings->setValue("framerateControl", ui->wiimoteFramerate->isChecked());
  m_settings->setValue("screenControl", ui->wiimoteScreen->isChecked());
  m_settings->endGroup();

  delete ui;
}
