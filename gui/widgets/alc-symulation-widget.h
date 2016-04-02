#pragma once

#include <QRgb>
#include <QVector>
#include <QWidget>

class QQuickItem;
class QQuickView;

class ALCSymulationWidget : public QWidget {
	Q_OBJECT
public:
	explicit ALCSymulationWidget(QWidget *parent = 0);
	virtual ~ALCSymulationWidget();

	//  void connectEmitter(Emitters::ALCEmitter *emitter);

	void onShow();

	void createQmlMonitor();
	void freeQmlMonitor();
	void createQmlObjects(int size = 300);
	void freeQmlObjects();
	void resetQmlObjects();

private:
	void createQmlObject(int ii, int i, QQuickItem *item, QObject *obj, int size);
	QString name();

protected:
	void timerEvent(QTimerEvent *);

private:
	QQuickItem *m_root;

	QObject *m_objs[4][8];
	QQuickItem *m_items[4][8];
	QQuickItem *m_monitor;
	QQuickView *m_view;
};
