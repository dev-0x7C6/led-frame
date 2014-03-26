#include "alc-screen-configure-dialog.h"
#include "ui_alc-screen-configure-dialog.h"

ALCScreenConfigureDialog::ALCScreenConfigureDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ALCScreenConfigureDialog)
{
  ui->setupUi(this);
}

ALCScreenConfigureDialog::~ALCScreenConfigureDialog()
{
  delete ui;
}
