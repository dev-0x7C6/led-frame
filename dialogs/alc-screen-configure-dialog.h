#pragma once

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
	class ALCScreenConfigureDialog;
}

class ALCScreenConfigureDialog : public QDialog {
	Q_OBJECT
private:
	Ui::ALCScreenConfigureDialog *ui;

public:
	explicit ALCScreenConfigureDialog(QWidget *parent = 0);
	~ALCScreenConfigureDialog();

	//  void setEmitter(Emitters::ALCScreenEmitter *);

private:
	void fpsValueChanged(int);
	void clipValueChanged(int);

	void accepted(QAbstractButton *);
};

