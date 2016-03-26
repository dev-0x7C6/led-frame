#pragma once

#include <QWidget>

namespace Ui {
	class ALCLedConfigurationWidget;
}

class ALCLedConfigurationWidget : public QWidget {
	Q_OBJECT
public:
	explicit ALCLedConfigurationWidget(QWidget *parent = 0);
	~ALCLedConfigurationWidget();

private:
	Ui::ALCLedConfigurationWidget *ui;
};
