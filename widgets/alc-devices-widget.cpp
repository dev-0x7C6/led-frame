#include "alc-devices-widget.h"
#include "ui_alc-devices-widget.h"

#include "connector/alc-device-thread.h"
#include "managers/alc-emitter-manager.h"
#include "connector/alc-device-manager.h"
#include "emitters/color-emitter.h"

#include <QCommandLinkButton>

ALCDevicesWidget::ALCDevicesWidget(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ALCDevicesWidget),
  m_manager(ALCDeviceManager::instance())
{
  ui->setupUi(this);
  connect(m_manager, &ALCDeviceManager::deviceConnected, this, &ALCDevicesWidget::deviceConnected, Qt::DirectConnection);
  connect(m_manager, &ALCDeviceManager::deviceDisconnected, this, &ALCDevicesWidget::deviceDisconnected, Qt::DirectConnection);

  connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, this, &ALCDevicesWidget::populate);
}

ALCDevicesWidget::~ALCDevicesWidget()
{
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


void ALCDevicesWidget::deviceConnected(ALCDeviceThread *thread) {
  ALCDeviceTreeWidget *item = new ALCDeviceTreeWidget(ui->tree, thread);
  connect(item, &ALCDeviceTreeWidget::setEmitter, this, &ALCDevicesWidget::setEmitter);
  item->setText(0, thread->details().systemLocation() + '\t');
  item->setIcon(0, QIcon(":/22x22/device.png"));

  QTreeWidgetItem *child = new QTreeWidgetItem(item);

  QWidget *workspace = new QWidget(0);
  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
  QPalette p = palette();
  p.setColor(QPalette::Background, QColor(226, 237, 253) );
  workspace->setPalette(p);
  workspace->setBackgroundRole(QPalette::Background);
  workspace->setLayout(layout);
  workspace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
  workspace->setAutoFillBackground(true);


  DeviceLinkButton *linkLedStrip = new DeviceLinkButton;
  DeviceLinkButton *linkEmitter = new DeviceLinkButton;
  DeviceLinkButton *linkDevice = new DeviceLinkButton;

  linkDevice->setDeviceThread(thread);
  linkDevice->setText("Device settings");
  linkDevice->setIconSize(QSize(22, 22));
  linkDevice->setIcon(QIcon(":/22x22/device.png"));
  linkDevice->setDescription("You can set there setings like: device brightness, update speed");
  linkDevice->setMaximumHeight(50);

  linkEmitter->setDeviceThread(thread);
  linkEmitter->setIconSize(QSize(22, 22));
  linkEmitter->setIcon(QIcon(":/22x22/leds.png"));
  linkEmitter->setText("Emitter settings");
  linkEmitter->setDescription("Quick settings for your currently selected emitter");
  linkEmitter->setMaximumHeight(50);
  connect(linkEmitter, &DeviceLinkButton::clicked, this, &ALCDevicesWidget::configureEmitter);

  linkLedStrip->setDeviceThread(thread);
  linkLedStrip->setIconSize(QSize(22, 22));
  linkLedStrip->setIcon(QIcon(":/22x22/chunks.png"));
  linkLedStrip->setText("Strip configuration");
  linkLedStrip->setDescription("Wizard configurator for led strips and your monitor");
  linkLedStrip->setMaximumHeight(50);

  linkDevice->setEnabled(false);
  linkLedStrip->setEnabled(false);

  layout->addWidget(linkDevice);
  layout->addWidget(linkEmitter);
  layout->addWidget(linkLedStrip);

  ui->tree->setItemWidget(child, 1, workspace);

  ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
  m_devices << item;

  populate();
}

void ALCDevicesWidget::deviceDisconnected(ALCDeviceThread *thread) {
  for (register int i = 0; i < m_devices.count(); ++i) {
    if (thread == m_devices[i]->device()) {
      delete m_devices[i];
      m_devices.removeAt(i);
      return;
    }
  }
}

void ALCDevicesWidget::populate() {
  const QString prefix = "Emitter: ";
  for (register int i = 0; i < m_devices.count(); ++i) {
    ui->tree->removeItemWidget(m_devices[i], 1);
    ComboBoxItem *cmb = new ComboBoxItem(m_devices[i], 1);
    cmb->setIconSize(QSize(22, 22));
    cmb->addItem(QIcon(":/22x22/no-device.png"), QString("Not assigned"));

    QList < ColorEmitter*> emitters = ALCEmitterManager::instance()->allEmitters();
    QListIterator < ColorEmitter*> ii(emitters);
    ColorEmitter *emitter;
    while (ii.hasNext()) {
      switch ((emitter = ii.next())->type()) {
      case ColorEmitter::EMITTER_SCREEN_CAPTURE:
        cmb->addItem(QIcon(":/22x22/screen.png"), prefix + emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      case ColorEmitter::EMITTER_PLAIN_COLOR:
        cmb->addItem(QIcon(":/22x22/color.png"),  prefix + emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      case ColorEmitter::EMITTER_ANIMATION:
        cmb->addItem(QIcon(":/22x22/animation.png"), prefix + emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      case ColorEmitter::EMITTER_IMAGE:
        cmb->addItem(QIcon(":/22x22/from-image.png"), prefix + emitter->emitterName(),
                     qVariantFromValue((void*)emitter));
        break;
      default:
        break;
      }
    }

    connect(cmb, static_cast < void( QComboBox::*)( int)>(&QComboBox::currentIndexChanged),
            m_devices[i], &ALCDeviceTreeWidget::currentIndexChanged, Qt::DirectConnection);

    ui->tree->setItemWidget(m_devices[i], 1, cmb);
  }
}

void ALCDevicesWidget::setEmitter(ALCDeviceThread *device, ColorEmitter *emitter) {
  device->connectEmitter(emitter);
}

#include <QMessageBox>

void ALCDevicesWidget::configureEmitter() {
  DeviceLinkButton *link = dynamic_cast < DeviceLinkButton*>( sender());
  ColorEmitter *emitter = link->deviceThread()->connectedEmitter();
  if (emitter)
    emitter->configure(); else
    QMessageBox::information(this, "Information", "Emitter is not defined.", QMessageBox::Ok);
}
