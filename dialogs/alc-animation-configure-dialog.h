#ifndef ALCANIMATIONCONFIGUREDIALOG_H
#define ALCANIMATIONCONFIGUREDIALOG_H

#include <QDialog>

class QShowEvent;
class ALCAnimationEmitter;

namespace Ui {
class ALCAnimationConfigureDialog;
}

class ALCAnimationConfigureDialog : public QDialog {
    Q_OBJECT
private:
    Ui::ALCAnimationConfigureDialog *ui;
    ALCAnimationEmitter *m_emitter;

public:
    explicit ALCAnimationConfigureDialog(QWidget *parent = 0);
    ~ALCAnimationConfigureDialog();

protected:
    void showEvent(QShowEvent *event);

private:
    void accept();
    void cancel();

    void blinkChanged(int);
    void speedChanged(int);

};

#endif // ALCANIMATIONCONFIGUREDIALOG_H
