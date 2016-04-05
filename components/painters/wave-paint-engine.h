#pragma once

#include <core/containers/color-scanline-container.h>

#include <QColor>
#include <QPainter>
#include <QTimer>

class QWidget;

namespace Widget {

	struct WaveGradientPoint final {
		double point;
		bool wavy;
		QColor color;
	};

	class WaveGradientModel final {
	public:
		explicit WaveGradientModel();
		virtual ~WaveGradientModel() = default;

		void setSinMultipler(double);

		void addPoint(qreal point, const QColor &color, bool wavy = true);
		void clear();
		QList <WaveGradientPoint> &points();

		qreal sinMultipler() const;

	private:
		QList <WaveGradientPoint> m_points;
		qreal m_sinMultipler;
	};

	class WavePaintEngine final {
	public:
		explicit WavePaintEngine(QWidget *parent);

		void paint(const QRect &rect, QPainter &painter);
		void resize(const QSize &size);

		bool animationEnabled() const;
		void setAnimationEnabled(bool animationEnabled);

		const Container::ColorScanlineContainer &scanline() const;

	protected:
		void update();

	private:
		bool m_animationEnabled;
		QTimer m_reflesh;
		int m_shift;
		QPixmap m_pixmap;
		QWidget *m_parent;
		Container::ColorScanlineContainer m_scanline;

		WaveGradientModel m_waveGradientModel;
	};

}
