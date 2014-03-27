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
  m_tray.setContextMenu(new QMenu());
  m_tray.setIcon(QIcon(":/22x22/leds.png"));
  m_tray.show();

  m_manager = new ALCDeviceManager(this);
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


 // connect(ui->framerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerate(int)));
  //connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));

  connect(ui->actionColor_correction, &QAction::toggled, this, &MainWindow::showColorCorrection);

  connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  //connect(ui->ledGlow, SIGNAL(valueChanged(int)), ui->widget, SLOT(setGlowSize(int)));
 // connect(ui->ledFramerateLimit, SIGNAL(valueChanged(int)), ui->widget, SLOT(setFramerate(int)));
  connect(ui->ledGlow, SIGNAL(valueChanged(int)), this, SLOT(setGlowSize(int)));
  connect(ui->ledFramerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerateLed(int)));

  m_settings->beginGroup("GeneralSettings");
  ui->actionColor_correction->setChecked(m_settings->value("visible", false).toBool());
 // ui->screenArea->setCurrentIndex(m_settings->value("screenId", 0).toInt());
 // ui->brightnessSlider->setValue(m_settings->value("brightness", 100).toInt());
 // ui->framerateLimit->setValue(m_settings->value("framerateLimit", 30).toInt());
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
  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, ui->selections, &ALCSelectionsWidget::setup, Qt::QueuedConnection);


  connect(m_manager, SIGNAL(deviceConnected(ALCDeviceThread*)), this, SLOT(deviceConnected(ALCDeviceThread*)), Qt::DirectConnection);
  connect(m_manager, SIGNAL(deviceDisconnected(ALCDeviceThread*)), this, SLOT(deviceDisconnected(ALCDeviceThread*)), Qt::DirectConnection);
  connect(ui->screenArea, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenArea(int)));

}

void MainWindow::deviceConnected(ALCDeviceThread *thread) {
  QRect rect;
  ALCDeviceTreeWidget *item = new ALCDeviceTreeWidget(ui->devices, thread);
  connect(item, &ALCDeviceTreeWidget::setEmitter, this, &MainWindow::setEmitter);
  item->setText(0, thread->details().systemLocation() + '\t');
  item->setIcon(0, QIcon(":/22x22/device.png"));


  QTreeWidgetItem *brightness = new QTreeWidgetItem(item);

  SliderItem *slider = new SliderItem(Qt::Horizontal, brightness, 1);
  brightness->setText(0, "Brightness");
  brightness->setIcon(0, QIcon(":/22x22/brightness.png"));
  ui->devices->setItemWidget(brightness, 1, slider);
  slider->setTickInterval(10);
 // slider->setTickPosition(QSlider::TicksBelow);
  slider->setMinimum(1);
  slider->setMaximum(200);
  slider->setValue(100);
  connect(slider, &QSlider::valueChanged, this, &MainWindow::setDeviceBrightness);

  QTreeWidgetItem *delay = new QTreeWidgetItem(item);

  SpinBoxItem *spinbox = new SpinBoxItem(brightness, 1);
  delay->setText(0, "I/O delay");
  delay->setIcon(0, QIcon(":/22x22/fpsrate.png"));
  ui->devices->setItemWidget(delay, 1, spinbox);
  spinbox->setMinimum(0);
  spinbox->setMaximum(1000);
  spinbox->setValue(0);
  spinbox->setSuffix(" ms");
 // spinbox->setMaximumWidth(70);

  connect(spinbox, static_cast< void(QSpinBox::*)(int) >(&QSpinBox::valueChanged),
          this, &MainWindow::setDeviceIODelay);

  delay = new QTreeWidgetItem(item);
  spinbox = new SpinBoxItem(brightness, 1);
  delay->setText(0, "Leds");
  delay->setIcon(0, QIcon(":/22x22/fpsrate.png"));
  ui->devices->setItemWidget(delay, 1, spinbox);
  spinbox->setMinimum(0);
  spinbox->setMaximum(1000);
  spinbox->setValue(0);
  spinbox->setSuffix(" leds");
  //spinbox->setMaximumWidth(70);

  for (register int i = 0; i < 4; ++i) {
    QTreeWidgetItem *a = new QTreeWidgetItem(delay);
    a->setText(0, "Stripe #" + QString::number(i+1));
    LedConfigurationItem *ledc = new LedConfigurationItem(a, 1);
    ui->devices->setItemWidget(a, 1, ledc);
  }


  QTreeWidgetItem *led = new QTreeWidgetItem(item);
  led->setText(0, "Color corretion");
  led->setIcon(0, QIcon("://22x22/color.png"));

  ComboBoxItem *cmb = new ComboBoxItem(led, 1);
  cmb->setIconSize(QSize(22, 22));
  cmb->addItem(QIcon(":/22x22/no-device.png"), QString("RGB"));
  cmb->addItem(QIcon(":/22x22/no-device.png"), QString("RBG"));
  cmb->addItem(QIcon(":/22x22/no-device.png"), QString("GRB"));
  cmb->addItem(QIcon(":/22x22/no-device.png"), QString("BRG"));
  cmb->addItem(QIcon(":/22x22/no-device.png"), QString("GBR"));
  cmb->addItem(QIcon(":/22x22/no-device.png"), QString("BGR"));
  ui->devices->setItemWidget(led, 1, cmb);
  connect(cmb, SIGNAL(currentIndexChanged(int)), this,
          SLOT(setDeviceColorFormat(int)), Qt::DirectConnection);

  QTreeWidgetItem *red = new QTreeWidgetItem(led);
  red->setText(0, "Red");
  red->setIcon(0, QIcon("://22x22/color.png"));
  slider = new SliderItem(Qt::Horizontal, red, 1);
  ui->devices->setItemWidget(red, 1, slider);
  slider->setTickInterval(10);
  slider->setTickPosition(QSlider::TicksBelow);
  slider->setMinimum(0);
  slider->setMaximum(200);
  slider->setValue(100);
  connect(slider, &QSlider::valueChanged, this, &MainWindow::setDeviceRedColorCorrection);


  QTreeWidgetItem *green = new QTreeWidgetItem(led);
  green->setText(0, "Green");
  green->setIcon(0, QIcon("://22x22/color.png"));
  slider = new SliderItem(Qt::Horizontal, green, 1);
  ui->devices->setItemWidget(green, 1, slider);
  slider->setTickInterval(10);
  slider->setTickPosition(QSlider::TicksBelow);
  slider->setMinimum(0);
  slider->setMaximum(200);
  slider->setValue(100);
  connect(slider, &QSlider::valueChanged, this, &MainWindow::setDeviceGreenColorCorrection);

  QTreeWidgetItem *blue = new QTreeWidgetItem(led);
  blue->setText(0, "Blue");
  blue->setIcon(0, QIcon("://22x22/color.png"));
  slider = new SliderItem(Qt::Horizontal, blue, 1);
  ui->devices->setItemWidget(blue, 1, slider);
  slider->setTickInterval(10);
  slider->setTickPosition(QSlider::TicksBelow);
  slider->setMinimum(0);
  slider->setMaximum(200);
  slider->setValue(100);
  connect(slider, &QSlider::valueChanged, this, &MainWindow::setDeviceBlueColorCorrection);

  ui->devices->header()->resizeSections(QHeaderView::ResizeToContents);
  m_devices << item;

  populate();
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

#include "emitters/color-emitter.h"

void MainWindow::populate() {
  for (register int i = 0; i < m_devices.count(); ++i) {
    ui->devices->removeItemWidget(m_devices[i], 1);
    ComboBoxItem *cmb = new ComboBoxItem(m_devices[i], 1);
    cmb->setIconSize(QSize(22, 22));
    cmb->addItem(QIcon(":/22x22/no-device.png"), QString("Not assigned"));

    QList < ColorEmitter*> emitters = ALCEmitterManager::instance()->allEmitters();
    QListIterator < ColorEmitter*> ii(emitters);
    ColorEmitter *emitter;
    while (ii.hasNext()) {
      switch ((emitter = ii.next())->type()) {
      case ColorEmitter::EMITTER_SCREEN_CAPTURE:
        cmb->addItem(QIcon(":/22x22/screen.png"), emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      case ColorEmitter::EMITTER_PLAIN_COLOR:
        cmb->addItem(QIcon(":/22x22/color.png"), emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      case ColorEmitter::EMITTER_ANIMATION:
        cmb->addItem(QIcon(":/22x22/animation.png"), emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      case ColorEmitter::EMITTER_IMAGE:
        cmb->addItem(QIcon(":/22x22/from-image.png"), emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      }
    }

    connect(cmb, static_cast < void( QComboBox::*)( int)>(&QComboBox::currentIndexChanged),
            m_devices[i], &ALCDeviceTreeWidget::currentIndexChanged, Qt::DirectConnection);

    ui->devices->setItemWidget(m_devices[i], 1, cmb);
  }
}

void ALCDeviceTreeWidget::currentIndexChanged(int idx) {
  Q_UNUSED(idx)
  ComboBoxItem *cmb = dynamic_cast < ComboBoxItem*> ( sender());
  emit setEmitter(m_device, reinterpret_cast< ColorEmitter*> (qvariant_cast < void*> (cmb->currentData())));
}


void MainWindow::setEmitter(ALCDeviceThread *device, ColorEmitter *emitter) {
  device->connectEmitter(emitter);
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

  delete m_manager;
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
  if(index >= 0) {
    item->setData(this->column, Qt::UserRole, itemText(index));
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


LedConfigurationItem::LedConfigurationItem(QTreeWidgetItem *, int)
{
  this->item = item;
  this->column = column;
}

void LedConfigurationItem::changeItem(int) { }
