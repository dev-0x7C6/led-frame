#ifndef ALCSCREENCONFIGUREDIALOG_H
#define ALCSCREENCONFIGUREDIALOG_H

#include <QDialog>

namespace Ui {
class ALCScreenConfigureDialog;
}

class ALCScreenConfigureDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ALCScreenConfigureDialog(QWidget *parent = 0);
  ~ALCScreenConfigureDialog();

private:
  Ui::ALCScreenConfigureDialog *ui;
};

#endif // ALCSCREENCONFIGUREDIALOG_H
