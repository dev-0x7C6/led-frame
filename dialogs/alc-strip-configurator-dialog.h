#ifndef ALCSTRIPCONFIGURATORDIALOG_H
#define ALCSTRIPCONFIGURATORDIALOG_H

#include <QDialog>

namespace Ui {
class ALCStripConfiguratorDialog;
}

class ALCStripConfiguratorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ALCStripConfiguratorDialog(QWidget *parent = 0);
    ~ALCStripConfiguratorDialog();

private:
    Ui::ALCStripConfiguratorDialog *ui;
};

#endif // ALCSTRIPCONFIGURATORDIALOG_H
