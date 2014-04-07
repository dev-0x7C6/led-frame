#ifndef ALCSCREENCONFIGUREDIALOG_H
#define ALCSCREENCONFIGUREDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class ALCScreenConfigureDialog;
}


class ScreenCaptureColorEmitter;

class ALCScreenConfigureDialog : public QDialog {
  Q_OBJECT
private:
  Ui::ALCScreenConfigureDialog *ui;
  ScreenCaptureColorEmitter *m_emitter;

public:
  explicit ALCScreenConfigureDialog(QWidget *parent = 0);
  ~ALCScreenConfigureDialog();

  void setEmitter(ScreenCaptureColorEmitter *);

private:
  void fpsValueChanged(int);
  void clipValueChanged(int);


  void accepted(QAbstractButton *);
};

#endif // ALCSCREENCONFIGUREDIALOG_H
