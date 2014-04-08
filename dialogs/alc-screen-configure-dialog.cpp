#include "alc-screen-configure-dialog.h"
#include "ui_alc-screen-configure-dialog.h"

ALCScreenConfigureDialog::ALCScreenConfigureDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ALCScreenConfigureDialog) {
  ui->setupUi(this);
  connect(ui->fps, &QSlider::valueChanged, this, &ALCScreenConfigureDialog::fpsValueChanged);
  connect(ui->clip, &QSlider::valueChanged, this, &ALCScreenConfigureDialog::clipValueChanged);
  connect(ui->buttons, &QDialogButtonBox::clicked, this, &ALCScreenConfigureDialog::accepted);
}

ALCScreenConfigureDialog::~ALCScreenConfigureDialog() {
  delete ui;
}

#include "emitters/screen-capture-color-emitter.h"


void ALCScreenConfigureDialog::setEmitter(ScreenCaptureColorEmitter *emitter) {
  m_emitter = emitter;

  ui->areaX->setValue(emitter->area().x());
  ui->areaY->setValue(emitter->area().y());
  ui->areaWidth->setValue(emitter->area().width());
  ui->areaHeight->setValue(emitter->area().height());

  ui->fps->setValue(emitter->framerateLimit());
  ui->clip->setValue(emitter->marginProcent() * 100.0);

  int chunk = emitter->chunk();

 // if (chunk <= 32)
 //   ui->sampleSize->setCurrentIndex();

}

void ALCScreenConfigureDialog::fpsValueChanged(int value) {
  ui->fpsLabel->setText(QString("%1 fps").arg(value));
  if (value > 60)
    ui->fpsCpu->setPixmap(QPixmap(":/custom/red.png")); else
  if (value > 25)
    ui->fpsCpu->setPixmap(QPixmap(":/custom/blue.png")); else
    ui->fpsCpu->setPixmap(QPixmap(":/custom/green.png"));
}

void ALCScreenConfigureDialog::clipValueChanged(int value) {
  ui->clipLabel->setText(QString("%1%").arg(QString::number(value)));
}

void ALCScreenConfigureDialog::accepted(QAbstractButton *button) {
  if (!m_emitter)
    return;

  QRect area;

  switch (ui->buttons->buttonRole(button)) {
  case QDialogButtonBox::ApplyRole:
    area.setX(ui->areaX->value());
    area.setY(ui->areaY->value());
    area.setWidth(ui->areaWidth->value());
    area.setHeight(ui->areaHeight->value());
    m_emitter->setFramerateLimit(ui->fps->value());
    m_emitter->setCaptureArea(area);
    m_emitter->setMarginProcent(ui->clip->value() / 100.0);
  default:
    close();
  }
}
