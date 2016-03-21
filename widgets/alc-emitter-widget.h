#ifndef ALCEMITTERWIDGET_H
#define ALCEMITTERWIDGET_H

#include <QMainWindow>
#include <QPushButton>

namespace Emitters {
	class ALCEmitter;
}

class QPushButtonEx : public QPushButton {
private:
	Emitters::ALCEmitter *m_emitter;

public:
	explicit QPushButtonEx(QWidget *parent = 0) :
		QPushButton(parent),
		m_emitter(0) {}

	void setEmitter(Emitters::ALCEmitter *emitter) {
		m_emitter = emitter;
	}

	Emitters::ALCEmitter *emitter() {
		return m_emitter;
	}
};


class QTreeWidgetItem;
class QBoxLayout;

namespace Ui {
	class ALCEmitterWidget;
}

class ALCEmitterWidget : public QMainWindow {
	Q_OBJECT
private:
	Ui::ALCEmitterWidget *ui;

public:
	explicit ALCEmitterWidget(QWidget *parent = 0);
	~ALCEmitterWidget();

	void addPlainColorItem();
	void addAnimationItem();
	void addImageItem();

	void prepare();
	void setup();

private:
	void insertPlainColorItem(Emitters::ALCEmitter *);
	void insertAnimationItem(Emitters::ALCEmitter *);
	void insertImageItem(Emitters::ALCEmitter *);

	void insertDefaultButtons(Emitters::ALCEmitter *, QBoxLayout *);

	void reconfigure();
	void rename();
	void remove();

	void prepareColorItem(QTreeWidgetItem *, QColor);
};

#endif // ALCEMITTERSWIDGET_H
