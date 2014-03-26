#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::About)
{
  ui->setupUi(this);
  ui->text->setText(ui->text->text().arg(QApplication::applicationDisplayName()));
}

About::~About()
{
  delete ui;
}
