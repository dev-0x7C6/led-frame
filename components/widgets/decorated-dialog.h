#pragma once

#include <components/painters/wave-paint-engine.h>

#include <QDialog>

namespace Widget {

class DecoratedDialog : public QDialog {
	Q_OBJECT
public:
	explicit DecoratedDialog(QWidget *parent = nullptr);
	~DecoratedDialog() override = default;

	void setGradientAnimationEnabled(bool enabled);

protected:
	void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent *) override;
	void showEvent(QShowEvent *) override;

	WavePaintEngine &wave();

private:
	WavePaintEngine m_wave;
};
}
