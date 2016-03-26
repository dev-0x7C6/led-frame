#pragma once

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
