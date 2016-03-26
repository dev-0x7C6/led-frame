#pragma once

#include <QWidget>

namespace Ui {
	class ALCColorCorrectionWidget;
}

namespace Correctors {
	class ALCColorCorrection;
}

class ALCColorCorrectionWidget : public QWidget {
	Q_OBJECT
private:
	Ui::ALCColorCorrectionWidget *ui;
	Correctors::ALCColorCorrection *m_correction;

public:
	explicit ALCColorCorrectionWidget(QWidget *parent = 0);
	~ALCColorCorrectionWidget();

	void setColorCorrection(Correctors::ALCColorCorrection *correction);
	Correctors::ALCColorCorrection *colorCorrection();
	void reload();
	void restore();

private:
	void brightnessSliderChanged(const int value);
	void blueSliderChanged(const int value);
	void greenSliderChanged(const int value);
	void redSliderChanged(const int value);

	void restoreBrightness();
	void restoreRed();
	void restoreGreen();
	void restoreBlue();

signals:
	void brightnessChanged(double value);

};
