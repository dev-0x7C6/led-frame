#pragma once

#include <components/painters/wave-paint-engine.h>

#include <QDialog>

namespace Widget {

	class DecoratedDialog : public QDialog {
		Q_OBJECT
	public:
		explicit DecoratedDialog(QWidget *parent = nullptr);
		virtual ~DecoratedDialog() = default;

		void setGradientAnimationEnabled(bool enabled);

	protected:
		void paintEvent(QPaintEvent *);
		void resizeEvent(QResizeEvent *);
		void showEvent(QShowEvent *);

		WavePaintEngine &wave();

	private:
		WavePaintEngine m_wave;

	};


}
