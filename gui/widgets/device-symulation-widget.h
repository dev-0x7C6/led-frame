#pragma once

#include <core/abstracts/abstract-receiver.h>

#include <QRgb>
#include <QVector>
#include <QWidget>

class QQuickItem;
class QQuickView;

#include <array>

namespace Widget {

	class DeviceSymulationWidget : public QWidget, public Abstract::AbstractReceiver {
		Q_OBJECT
	public:
		explicit DeviceSymulationWidget(QWidget *parent = 0);
		virtual ~DeviceSymulationWidget();

		virtual QString name() const override;
		virtual Enum::ReceiverType type() const override;
		virtual Container::DeviceConfigContainer config() override;

		void createQmlMonitor();

		void createQmlRibbon();
		void resizeQmlRibbon(QSize area, const int &size = 256);
		void resizeQmlMonitor(QSize area);

	protected:
		virtual void timerEvent(QTimerEvent *event) override;
		virtual void resizeEvent(QResizeEvent *event) override;

	private:
		std::array<QQuickItem *, 24> m_leds;
		QQuickItem *m_monitor;
		QQuickView *m_view;
	};

}
