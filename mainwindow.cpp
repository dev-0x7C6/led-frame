#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include "about.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  m_settings(new QSettings("AmbientLedDriver", "AmbientLedDriver", this)),
#ifdef Q_OS_UNIX
  m_wiimotedevEvents(new WiimotedevDeviceEvents),
  m_buttons(0),
#endif
  ui(new Ui::MainWindow)
{
  qRegisterMetaType< QList<QRgb> >("QList< QRgb >");
  ui->setupUi(this);

  m_statisticAverageFPS = 0;
  m_statisticAverageLatency = 0;
  m_statisticAverageThreadUse = 0;
  m_statisticClock = 0;
  m_statisticFirstTime = true;

  QRect rect = QApplication::desktop()->geometry();
  ui->screenArea->addItem(QIcon(":/16x16/all-screens.png"),
    QString("Visible area, x:%1, y:%2 (%3x%4)").arg(
      QString::number(rect.x()),
      QString::number(rect.y()),
      QString::number(rect.width()),
      QString::number(rect.height())),
    -1);

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

  ui->leftWidget->setCurrentIndex(0);

  connect(ui->framerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerate(int)));
  connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
  connect(ui->screenArea, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenArea(int)));
  connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(ui->ledGlow, SIGNAL(valueChanged(int)), ui->widget, SLOT(setGlowSize(int)));
  connect(ui->ledFramerateLimit, SIGNAL(valueChanged(int)), ui->widget, SLOT(setFramerate(int)));
  connect(ui->ledGlow, SIGNAL(valueChanged(int)), this, SLOT(setGlowSize(int)));
  connect(ui->ledFramerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerateLed(int)));

  m_settings->beginGroup("GeneralSettings");
  ui->screenArea->setCurrentIndex(m_settings->value("screenId", 0).toInt());
  ui->brightnessSlider->setValue(m_settings->value("brightness", 100).toInt());
  ui->framerateLimit->setValue(m_settings->value("framerateLimit", 30).toInt());
  m_settings->endGroup();

  m_settings->beginGroup("AdvancedSettings");
  ui->chunkSize->setValue(m_settings->value("chunkSize", 48).toInt());
  ui->pixelSkip->setValue(m_settings->value("pixelSkip", 4).toInt());
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

  updateScreenArea(ui->screenArea->currentIndex());

#ifdef Q_OS_UNIX
  connect(m_wiimotedevEvents, SIGNAL(dbusWiimoteButtons(uint,uint64)), this, SLOT(dbusWiimotedevButtons(uint, uint64)));
#endif

  connect(&capture, SIGNAL(updateLeds(QList<QRgb>)), ui->widget, SLOT(updateLeds(QList<QRgb>)), Qt::QueuedConnection);
  connect(&capture, SIGNAL(updateStats(quint32,double,double)), this, SLOT(updateStats(quint32,double,double)), Qt::QueuedConnection);
  connect(ui->chunkSize, SIGNAL(valueChanged(int)), &capture, SLOT(setChunkSize(int)), Qt::DirectConnection);
  connect(ui->pixelSkip, SIGNAL(valueChanged(int)), &capture, SLOT(setPixelSkip(int)), Qt::DirectConnection);

  capture.setFramerateLimit(ui->framerateLimit->value());
  capture.setPixelSkip(ui->pixelSkip->value());
}

void MainWindow::showEvent(QShowEvent *) {
  if (!capture.isRunning())
    capture.start(QThread::HighPriority);
}

void MainWindow::setGlowSize(int value) {
  ui->ledGlowLabel->setText(QString::number(value) + "px");
}

void MainWindow::setFramerateLed(int value) {
  ui->ledFps->setText(QString::number(value) + "fps");
}

void MainWindow::setFramerate(int value) {
  ui->fpsCount->setText(QString::number(value) + "fps");
  capture.setFramerateLimit(value);
}

void MainWindow::setBrightness(int value) {
  double brightness = double(value) / 100.0;
  ui->proc->setText(QString::number(int(brightness*100)) + "%");
  capture.setBrightness(brightness);
}

void MainWindow:: updateScreenArea(int area) {
  QRect geometry;
  if (area == 0)
    geometry = QApplication::desktop()->geometry(); else
    geometry = QApplication::desktop()->screenGeometry(area - 1);

  ui->x->setValue(0);
  ui->y->setValue(0);
  ui->xoffset->setValue(geometry.width());
  ui->yoffset->setValue(geometry.height());

  geometry.setCoords(ui->x->value() + geometry.x(),
                     ui->y->value() + geometry.y(),
                     ui->xoffset->value() + geometry.x() - 1,
                     ui->yoffset->value() + geometry.y() - 1);

  capture.setCaptureArea(geometry);
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

#ifdef Q_OS_UNIX
void MainWindow::dbusWiimotedevButtons(uint id, uint64 buttons) {
  if (int(id) != ui->wiimoteId->value())
    return;

  if (ui->wiimoteBrightness->isChecked()) {
    if ((buttons & WIIMOTE_BTN_PLUS) && !(m_buttons & WIIMOTE_BTN_PLUS))
      ui->brightnessSlider->setValue(ui->brightnessSlider->value() + 10);
    if ((buttons & WIIMOTE_BTN_MINUS) && !(m_buttons & WIIMOTE_BTN_MINUS))
      ui->brightnessSlider->setValue(ui->brightnessSlider->value() - 10);
  }

  if (ui->wiimoteFramerate->isChecked()) {
    if ((buttons & WIIMOTE_BTN_RIGHT) && !(m_buttons & WIIMOTE_BTN_RIGHT))
      ui->framerateLimit->setValue(ui->framerateLimit->value() + 2);
    if ((buttons & WIIMOTE_BTN_LEFT) && !(m_buttons & WIIMOTE_BTN_LEFT))
      ui->framerateLimit->setValue(ui->framerateLimit->value() - 2);
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


MainWindow::~MainWindow()
{
  capture.setQuitState(true);
  capture.wait();

  m_settings->beginGroup("GeneralSettings");
  m_settings->setValue("screenId", ui->screenArea->currentIndex());
  m_settings->setValue("brightness", ui->brightnessSlider->value());
  m_settings->setValue("framerateLimit", ui->framerateLimit->value());
  m_settings->endGroup();

  m_settings->beginGroup("AdvancedSettings");
  m_settings->setValue("chunkSize", ui->chunkSize->value());
  m_settings->setValue("pixelSkip", ui->pixelSkip->value());
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
