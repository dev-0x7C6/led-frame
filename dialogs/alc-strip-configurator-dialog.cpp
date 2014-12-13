#include "alc-strip-configurator-dialog.h"
#include "ui_alc-strip-configurator-dialog.h"

ALCStripConfiguratorDialog::ALCStripConfiguratorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ALCStripConfiguratorDialog) {
    ui->setupUi(this);
}

ALCStripConfiguratorDialog::~ALCStripConfiguratorDialog() {
    delete ui;
}
