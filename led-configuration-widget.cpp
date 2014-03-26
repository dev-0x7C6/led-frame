#include "led-configuration-widget.h"
#include "ui_led-configuration-widget.h"

LedConfigurationWidget::LedConfigurationWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::LedConfigurationWidget)
{
  ui->setupUi(this);
}

LedConfigurationWidget::~LedConfigurationWidget()
{
  delete ui;
}
