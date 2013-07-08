#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include "about.h"



ComboBoxItem::ComboBoxItem(QTreeWidgetItem *item, int column)
{
    this->item = item;
    this->column = column;
    connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeItem(int)));
}

void ComboBoxItem::changeItem(int index)
{
    if(index >=0)
    {
        item->setData(this->column, Qt::UserRole, this->itemText(index));
        //item->data(this->column, Qt::UserRole).toString();
    }
}

RadioButtonItem::RadioButtonItem(QTreeWidgetItem *item, int column)
{
  this->setText("Yes");
    this->item = item;
    this->column = column;
    connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeItem(int)));
}

void RadioButtonItem::changeItem(int index)
{
    if(index >=0)
    {
      item->setData(this->column, Qt::UserRole, "44");
        //item->data(this->column, Qt::UserRole).toString();
    }
}

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


  for (register int i = 0; i < 10; ++i) {
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0, QString("/dev/ttyUSB%1\t").arg(QString::number(i)));
    item->setIcon(0, QIcon(":/22x22/device.png"));

    ComboBoxItem *cmb = new ComboBoxItem(item, 1);
    cmb->setIconSize(QSize(22, 22));
    cmb->addItem(QIcon(":/22x22/no-device.png"), QString("Not assigned"));

    for (register int i = -2; i < QApplication::desktop()->screenCount(); ++i) {
      if (i == -1 || i == -2)
        rect = QApplication::desktop()->geometry(); else
        rect = QApplication::desktop()->screenGeometry(i);

      QString str = QString("(%3x%4) x:%1, y:%2").arg(QString::number(rect.x()),
                                                             QString::number(rect.y()),
                                                             QString::number(rect.width()),
                                                             QString::number(rect.height()));




      switch (i) {
      case -2:
        cmb->addItem(QIcon(":/22x22/selected-area.png"), QString("Selected area: " + str));
        break;
      case -1:
        cmb->addItem(QIcon(":/22x22/all-screens.png"), QString("Visible area: "  + str));
        break;
      default:
        cmb->addItem(QIcon(":/22x22/screen.png"), QString("Screen %1: "  + str).arg(QString::number(i)));
      }

    }

    for (register int i = 1; i < 7; ++i) {
      cmb->addItem(QIcon(":/22x22/color.png"), QString("Solid color profile #%1").arg(QString::number(i)));
    }
    for (register int i = 1; i < 7; ++i) {
      cmb->addItem(QIcon(":/22x22/animation.png"), QString("Animation color profile #%1").arg(QString::number(i)));
    }

    ui->treeWidget->setItemWidget(item, 1, cmb);
    RadioButtonItem *rdi = new RadioButtonItem(item, 1);
    rdi->setIconSize(QSize(22, 22));
    ui->treeWidget->setItemWidget(item, 2, rdi);




  }




//  for (register int i = -2; i < QApplication::desktop()->screenCount(); ++i) {
//    if (i == -1 || i == -2)
//      rect = QApplication::desktop()->geometry(); else
//      rect = QApplication::desktop()->screenGeometry(i);

//    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);


//    switch (i) {
//    case -2:
//      item->setText(0, QString("Selected area"));
//      item->setIcon(0, QIcon(":/22x22/selected-area.png"));
//      break;
//    case -1:
//      item->setText(0, QString("Visible area"));
//      item->setIcon(0, QIcon(":/22x22/all-screens.png"));
//      break;
//    default:
//      item->setText(0, QString("Screen %1").arg(QString::number(i)));
//      item->setIcon(0, QIcon(":/22x22/screen.png"));
//    }







//    cmb->addItem(QIcon(":/22x22/no-device.png"), "disabled");
//    cmb->addItem(QIcon(":/22x22/device.png"), "/dev/ttyUSB0");
//    cmb->addItem(QIcon(":/22x22/device.png"), "/dev/ttyUSB1");
//    cmb->addItem(QIcon(":/22x22/device.png"), "/dev/ttyUSB2");
//    ui->treeWidget->setItemWidget(item, 2, cmb);

//
//  }


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

ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);

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
  connect(&capture, SIGNAL(updateLeds(QList<QRgb>)), &m_backend, SLOT(updateLeds(QList<QRgb>)), Qt::DirectConnection);
  connect(&capture, SIGNAL(updateStats(quint32,double,double)), this, SLOT(updateStats(quint32,double,double)), Qt::QueuedConnection);
  connect(ui->chunkSize, SIGNAL(valueChanged(int)), &capture, SLOT(setChunkSize(int)), Qt::DirectConnection);
  connect(ui->pixelSkip, SIGNAL(valueChanged(int)), &capture, SLOT(setPixelSkip(int)), Qt::DirectConnection);

  capture.setFramerateLimit(ui->framerateLimit->value());
  capture.setPixelSkip(ui->pixelSkip->value());
  capture.setChunkSize(ui->chunkSize->value());





  m_backend.start();
}

void MainWindow::showEvent(QShowEvent *) {
  if (!capture.isRunning()) {
    capture.start();
//    capture.wait(100);

//  CaptureThread *cp1 = new CaptureThread();
//  for (register int i = 0; i < 20; ++i) {


//    cp1->setCaptureArea(QRect(0, 0, 1000, 1000));
//    cp1->setFramerateLimit(30);
//    cp1->setChunkSize(20);
//    cp1->setPixelSkip(2);
//    cp1->start();
//    cp1->wait(10);
//    qDebug() << cp1;
//    cp1 = new CaptureThread();

//  }
  }
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
