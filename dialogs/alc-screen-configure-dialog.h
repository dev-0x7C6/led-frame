#ifndef ALCSCREENCONFIGUREDIALOG_H
#define ALCSCREENCONFIGUREDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
	class ALCScreenConfigureDialog;
}

#include "emitters/alc-screen-emitter.h"

class ALCScreenConfigureDialog : public QDialog {
	Q_OBJECT
private:
	Ui::ALCScreenConfigureDialog *ui;
	Emitters::ALCScreenEmitter *m_emitter;

public:
	explicit ALCScreenConfigureDialog(QWidget *parent = 0);
	~ALCScreenConfigureDialog();

	void setEmitter(Emitters::ALCScreenEmitter *);

private:
	void fpsValueChanged(int);
	void clipValueChanged(int);

	void accepted(QAbstractButton *);
};

#endif // ALCSCREENCONFIGUREDIALOG_H
