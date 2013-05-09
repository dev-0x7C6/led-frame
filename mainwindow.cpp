#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QElapsedTimer>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
#ifdef Q_OS_UNIX
  ,
  m_wiimotedevEvents(new WiimotedevDeviceEvents()),
  m_buttons(0)
#endif
{
  ui->setupUi(this);

  QString info = " x:" +
      QString::number(QApplication::desktop()->geometry().x()) + ", y:" +
      QString::number(QApplication::desktop()->geometry().y()) + " (" +
      QString::number(QApplication::desktop()->geometry().width()) + "x" +
      QString::number(QApplication::desktop()->geometry().height()) + ")";
  ui->screenArea->addItem(QIcon(":/16x16/all-screens.png"), "Visible area, " + info, -1);
  for (register int i = 0; i < QApplication::desktop()->screenCount(); ++i) {
    info = " x:" +
        QString::number(QApplication::desktop()->screenGeometry(i).x()) + ", y:" +
        QString::number(QApplication::desktop()->screenGeometry(i).y()) + " (" +
        QString::number(QApplication::desktop()->screenGeometry(i).width()) + "x" +
        QString::number(QApplication::desktop()->screenGeometry(i).height()) + ")";
    ui->screenArea->addItem(QIcon(":/16x16/selected-screen.png"), "Screen " + QString::number(i) + ": "  + info, i);
  }

  updateScreenArea(0);
  setFramerate(30);

  connect(ui->framerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerate(int)));
  connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
  connect(ui->screenArea, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenArea(int)));

#ifdef Q_OS_UNIX
  connect(m_wiimotedevEvents, SIGNAL(dbusWiimoteButtons(uint,uint64)), this, SLOT(dbusWiimotedevButtons(uint, uint64)));
#endif

  m_title = windowTitle();

  connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

  qRegisterMetaType< QList<QRgb> >("QList< QRgb >");

  connect(&capture, SIGNAL(updateLeds(QList<QRgb>)), ui->widget, SLOT(updateLeds(QList<QRgb>)), Qt::QueuedConnection);
  connect(&capture, SIGNAL(updateStats(quint32,double,double)), this, SLOT(updateStats(quint32,double,double)), Qt::QueuedConnection);
  connect(ui->chunkSize, SIGNAL(valueChanged(int)), &capture, SLOT(setChunkSize(int)), Qt::DirectConnection);
  connect(ui->pixelSkip, SIGNAL(valueChanged(int)), &capture, SLOT(setPixelSkip(int)), Qt::DirectConnection);
  connect(ui->alghoritm, SIGNAL(currentIndexChanged(int)), &capture, SLOT(setAlghoritm(int)), Qt::DirectConnection);
  capture.setCaptureArea(QApplication::desktop()->screenGeometry(-1));
  capture.start();
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
  if (area == 0) {
    geometry = QApplication::desktop()->geometry();
  } else {
    geometry = QApplication::desktop()->screenGeometry(area-1);
  }

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
  setWindowTitle(m_title + " (" + QString::number(fps) + "fps) latency: " + QString::number(latency, 'f', 1)+"ms, thread usage: " + QString::number(int(usage)) + "%");
}

#include "about.h"

void MainWindow::about() {
  About form(this);
  form.exec();
}

#ifdef Q_OS_UNIX
void MainWindow::dbusWiimotedevButtons(uint id, uint64 buttons) {
  if (id != ui->wiimoteId->value())
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
  delete ui;
}
