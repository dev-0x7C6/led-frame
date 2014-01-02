#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>
#include "about.h"

#include "emitters/blackholecoloremitter.h"
#include "connector/alc-device-thread.h"
#include "connector/alc-device-manager.h"

#include <QQuickView>

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

//  QQuickView *view = new QQuickView();
//  QWidget *container = QWidget::createWindowContainer(view, this);
//  container->setMinimumSize(500, 300);
//  container->setMaximumSize(500, 300);
//  container->setFocusPolicy(Qt::TabFocus);
//  view->setSource(QUrl("qrc:/qml/main.qml"));
//  ui->qml->addWidget(container);


  m_manager = new ALCDeviceManager(this);
  QRect rect;


  m_statisticAverageFPS = 0;
  m_statisticAverageLatency = 0;
  m_statisticAverageThreadUse = 0;
  m_statisticClock = 0;
  m_statisticFirstTime = true;


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

  for (register int i = 0; i < 8; ++i) {
    ui->screenArea->addItem(QIcon(":/22x22/animation.png"),
      QString("Animation color profile #%1").arg(QString::number(i+1)),
      i);
  }

  ui->leftWidget->setCurrentIndex(0);


 // connect(ui->framerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerate(int)));
  connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));

  connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(ui->ledGlow, SIGNAL(valueChanged(int)), ui->widget, SLOT(setGlowSize(int)));
  connect(ui->ledFramerateLimit, SIGNAL(valueChanged(int)), ui->widget, SLOT(setFramerate(int)));
  connect(ui->ledGlow, SIGNAL(valueChanged(int)), this, SLOT(setGlowSize(int)));
  connect(ui->ledFramerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerateLed(int)));

  m_settings->beginGroup("GeneralSettings");
 // ui->screenArea->setCurrentIndex(m_settings->value("screenId", 0).toInt());
  ui->brightnessSlider->setValue(m_settings->value("brightness", 100).toInt());
 // ui->framerateLimit->setValue(m_settings->value("framerateLimit", 30).toInt());
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


#ifdef Q_OS_UNIX
  connect(m_wiimotedevEvents, SIGNAL(dbusWiimoteButtons(uint,uint64)), this, SLOT(dbusWiimotedevButtons(uint, uint64)));
#endif

  connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));

  m_colorEmitters << new BlackholeColorEmitter(0);

  for (register int i = -2; i < QApplication::desktop()->screenCount(); ++i) {
    ScreenCaptureColorEmitter *capture = new ScreenCaptureColorEmitter(0);

    if (i == -1 || i == -2)
      rect = QApplication::desktop()->geometry(); else
      rect = QApplication::desktop()->screenGeometry(i);

    capture->setCaptureArea(rect);
    capture->setPixelSkip(4);
    capture->setChunkSize(160);
    capture->setFramerateLimit(60);
    capture->setBrightness(ui->brightnessSlider->value());
    capture->start();

    dynamic_cast< ColorEmitter*>(capture)->setBrightness(ui->brightnessSlider->value());

    m_colorEmitters << dynamic_cast< ColorEmitter*>(capture);
  }

  AnimationColorEmitter *animation;
  for (register int i = 0; i < 8; ++i) {
    animation = new AnimationColorEmitter();
    m_colorEmitters << dynamic_cast< ColorEmitter*>(animation);
  }

  setBrightness(ui->brightnessSlider->value());

  connect(ui->brightnessBoost, &QCheckBox::clicked, this, &MainWindow::setBrightnessBoost);

  connect(m_manager, SIGNAL(deviceConnected(ALCDeviceThread*)), this, SLOT(deviceConnected(ALCDeviceThread*)), Qt::DirectConnection);
  connect(m_manager, SIGNAL(deviceDisconnected(ALCDeviceThread*)), this, SLOT(deviceDisconnected(ALCDeviceThread*)), Qt::DirectConnection);
  connect(ui->screenArea, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenArea(int)));
}

void MainWindow::deviceConnected(ALCDeviceThread *thread) {
  QRect rect;
  ALCDeviceTreeWidget *item = new ALCDeviceTreeWidget(ui->treeWidget, thread);
  connect(item, SIGNAL(setCustomEmitter(ALCDeviceThread*,int)),
          this, SLOT(setCustomEmitter(ALCDeviceThread*,int)));
  item->setText(0, thread->details().systemLocation() + '\t');
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

  for (register int i = 1; i <= 8; ++i) {
    cmb->addItem(QIcon(":/22x22/animation.png"), QString("Animation color profile #%1").arg(QString::number(i)));
  }

  connect(cmb, SIGNAL(currentIndexChanged(int)), item, SLOT(currentIndexChanged(int)), Qt::DirectConnection);

  ui->treeWidget->setItemWidget(item, 1, cmb);
  ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);


  QTreeWidgetItem *brightness = new QTreeWidgetItem(item);

  SliderItem *slider = new SliderItem(Qt::Horizontal, brightness, 1);
  brightness->setText(0, "Brightness");
  brightness->setIcon(0, QIcon(":/22x22/brightness.png"));
  ui->treeWidget->setItemWidget(brightness, 1, slider);
  slider->setTickInterval(10);
 // slider->setTickPosition(QSlider::TicksBelow);
  slider->setMinimum(1);
  slider->setMaximum(200);
  slider->setValue(100);

  QTreeWidgetItem *delay = new QTreeWidgetItem(item);

  SpinBoxItem *spinbox = new SpinBoxItem(brightness, 1);
  delay->setText(0, "I/O delay");
  delay->setIcon(0, QIcon(":/22x22/fpsrate.png"));
  ui->treeWidget->setItemWidget(delay, 1, spinbox);
  spinbox->setMinimum(0);
  spinbox->setMaximum(100);
  spinbox->setValue(8);
  spinbox->setSuffix(" ms");
  spinbox->setMaximumWidth(70);

  m_devices << item;
}

void ALCDeviceTreeWidget::currentIndexChanged(int idx) {
  emit setCustomEmitter(m_device, idx);
}

void MainWindow::setCustomEmitter(ALCDeviceThread* device, int idx) {
  device->connectEmitter(m_colorEmitters[idx]);
}

void MainWindow::deviceDisconnected(ALCDeviceThread *thread) {

  for (register int i = 0; i < m_devices.count(); ++i) {
    if (thread == m_devices[i]->device()) {
      delete m_devices[i];
      m_devices.removeAt(i);
      return;
    }
  }
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

void MainWindow::setBrightness(int value) {
  double brightness = double(value) / 100.0;
  if (ui->brightnessBoost->isChecked())
    brightness *= 2.5;
  ui->proc->setText(QString::number(int(brightness*100)) + "%");

  for (register int i = 0; i < m_colorEmitters.count(); ++i)
    m_colorEmitters[i]->setBrightness(brightness);

  ////////////////////////////////////////capture.setBrightness(brightness);
}

void MainWindow::setBrightnessBoost(bool value)
{
  setBrightness(ui->brightnessSlider->value());
}

void MainWindow:: updateScreenArea(int area) {
//  QRect geometry;
//  if (area == 0)
//    geometry = QApplication::desktop()->geometry(); else
//    geometry = QApplication::desktop()->screenGeometry(area - 1);

//  ui->x->setValue(0);
//  ui->y->setValue(0);
//  ui->xoffset->setValue(geometry.width());
//  ui->yoffset->setValue(geometry.height());

//  geometry.setCoords(ui->x->value() + geometry.x(),
//                     ui->y->value() + geometry.y(),
//                     ui->xoffset->value() + geometry.x() - 1,
//                     ui->yoffset->value() + geometry.y() - 1);

  //qDebug() << area;

  ui->widget->connectEmitter(m_colorEmitters[area]);

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
      ui->brightnessSlider->setValue(ui->brightnessSlider->value() + 4);
    if ((buttons & WIIMOTE_BTN_MINUS) && !(m_buttons & WIIMOTE_BTN_MINUS))
      ui->brightnessSlider->setValue(ui->brightnessSlider->value() - 4);
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


MainWindow::~MainWindow()
{
  m_settings->beginGroup("GeneralSettings");
  m_settings->setValue("screenId", ui->screenArea->currentIndex());
  m_settings->setValue("brightness", ui->brightnessSlider->value());
//  m_settings->setValue("framerateLimit", ui->framerateLimit->value());
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

SliderItem::SliderItem(Qt::Orientation orientation, QTreeWidgetItem *item, int column) :
  QSlider(orientation, 0)
{
  this->item = item;
  this->column = column;
  connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeItem(int)));
}

void SliderItem::changeItem(int index) {
  if(index >=0)
    item->setData(this->column, Qt::UserRole, "44");
}


SpinBoxItem::SpinBoxItem(QTreeWidgetItem *item, int column)
{
  this->item = item;
  this->column = column;
  connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeItem(int)));
}

void SpinBoxItem::changeItem(int index)
{
  if(index >=0)
    item->setData(this->column, Qt::UserRole, "44");
}
