#include "alc-animation-configure-dialog.h"
#include "ui_alc-animation-configure-dialog.h"

#include "emitters/alc-animation-emitter.h"
#include "managers/alc-device-manager.h"
#include "connector/alc-device-thread.h"

ALCAnimationConfigureDialog::ALCAnimationConfigureDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ALCAnimationConfigureDialog),
  m_emitter(new ALCAnimationEmitter()) {
  ui->setupUi(this);
  connect(ui->buttons, &QDialogButtonBox::accepted, this, &ALCAnimationConfigureDialog::accept);
  connect(ui->buttons, &QDialogButtonBox::rejected, this, &ALCAnimationConfigureDialog::cancel);
  connect(ui->blinkSlider, &QSlider::valueChanged, this, &ALCAnimationConfigureDialog::blinkChanged);
  connect(ui->speedSlider, &QSlider::valueChanged, this, &ALCAnimationConfigureDialog::speedChanged);
  ui->qml->connectEmitter(m_emitter);
  ui->colorCorrection->setColorCorrection(m_emitter);
  ALCDeviceManager::instance()->device(0)->connectEmitter(m_emitter);
}

ALCAnimationConfigureDialog::~ALCAnimationConfigureDialog() {
  ui->qml->connectEmitter(0);
  ALCDeviceManager::instance()->device(0)->connectEmitter(0);
  delete ui;
  delete m_emitter;
}

void ALCAnimationConfigureDialog::showEvent(QShowEvent *event) {
  Q_UNUSED(event);
  setMaximumHeight(height());
  setMinimumHeight(height());
  setMaximumWidth(width());
  setMinimumWidth(width());
}

void ALCAnimationConfigureDialog::accept() {
  close();
}

void ALCAnimationConfigureDialog::cancel() {
  close();
}

void ALCAnimationConfigureDialog::blinkChanged(int) {
}

void ALCAnimationConfigureDialog::speedChanged(int speed) {
  m_emitter->rotatePalette(speed * 10);
}
