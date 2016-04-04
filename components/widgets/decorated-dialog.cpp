#include <components/painters/wave-paint-engine.h>
#include <components/widgets/decorated-dialog.h>

#include <QPainter>
#include <QResizeEvent>

namespace Widget {

	DecoratedDialog::DecoratedDialog(QWidget *parent) :
		QDialog(parent),
		m_wave(this) {
	}

	void DecoratedDialog::setGradientAnimationEnabled(bool enabled) {
		m_wave.setAnimationEnabled(enabled);
	}

	void DecoratedDialog::paintEvent(QPaintEvent *) {
		QPainter painter(this);
		m_wave.paint(rect(), painter);
	}

	void DecoratedDialog::resizeEvent(QResizeEvent *event) {
		m_wave.resize(event->size());
	}

	void DecoratedDialog::showEvent(QShowEvent *) {
		m_wave.resize(size());
	}

	WavePaintEngine &DecoratedDialog::wave() {
		return m_wave;
	}

}

