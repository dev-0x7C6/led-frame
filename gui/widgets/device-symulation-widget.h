#pragma once

#include <core/abstracts/abstract-receiver.h>

#include <QRgb>
#include <QVector>
#include <QWidget>

class QQuickItem;
class QQuickView;

namespace Widget {

	class DeviceSymulationWidget : public QWidget, public Abstract::AbstractReceiver {
		Q_OBJECT
	public:
		explicit DeviceSymulationWidget(QWidget *parent = 0);
		virtual ~DeviceSymulationWidget();

		virtual QString name() const override;
		virtual Enum::ReceiverType type() const override;
		virtual Container::DeviceConfigContainer config() override;

		void onShow();

		void createQmlMonitor();
		void freeQmlMonitor();
		void createQmlObjects(int size = 300);
		void freeQmlObjects();
		void resetQmlObjects();

	private:
		void createQmlObject(int ii, int i, QQuickItem *item, QObject *obj, int size);

	protected:
		virtual void timerEvent(QTimerEvent *) override;

	private:
		QQuickItem *m_root;

		QObject *m_objs[4][8];
		QQuickItem *m_items[4][8];
		QQuickItem *m_monitor;
		QQuickView *m_view;
	};

}
