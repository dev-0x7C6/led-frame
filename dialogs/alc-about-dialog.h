#ifndef ALCABOUTDIALOG_H
#define ALCABOUTDIALOG_H

#include <QDialog>

namespace Ui { class ALCAboutDialog; }

class ALCAboutDialog : public QDialog {
  Q_OBJECT
public:
  explicit ALCAboutDialog(QWidget *parent = 0);
  ~ALCAboutDialog();
  
private:
  Ui::ALCAboutDialog *ui;
};

#endif // ALCABOUTDIALOG_H
