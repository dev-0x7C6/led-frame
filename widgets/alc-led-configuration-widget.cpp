#include "alc-led-configuration-widget.h"
#include "ui_alc-led-configuration-widget.h"

ALCLedConfigurationWidget::ALCLedConfigurationWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ALCLedConfigurationWidget)
{
  ui->setupUi(this);
}

ALCLedConfigurationWidget::~ALCLedConfigurationWidget()
{
  delete ui;
}
