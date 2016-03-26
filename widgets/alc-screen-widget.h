#pragma once

#include <QMainWindow>
#include <QPushButton>

namespace Emitters {
	class ALCEmitter;
}

namespace Ui {
	class ALCScreenWidget;
}

class ALCScreenWidget : public QMainWindow {
	Q_OBJECT
private:
	Ui::ALCScreenWidget *ui;

public:
	explicit ALCScreenWidget(QWidget *parent = 0);
	~ALCScreenWidget();
	void setup();

private:
	void insertScreenCaptureItem(Emitters::ALCEmitter *);
	void configure();

};
