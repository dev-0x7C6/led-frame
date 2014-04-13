#include "alc-about-dialog.h"
#include "ui_alc-about-dialog.h"

ALCAboutDialog::ALCAboutDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ALCAboutDialog) {
  ui->setupUi(this);
  ui->text->setText(ui->text->text().arg(QApplication::applicationDisplayName()));
}

ALCAboutDialog::~ALCAboutDialog() {
  delete ui;
}
