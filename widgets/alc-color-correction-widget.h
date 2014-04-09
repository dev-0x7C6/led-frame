#ifndef ALCCOLORCORRECTIONWIDGET_H
#define ALCCOLORCORRECTIONWIDGET_H

#include <QWidget>

namespace Ui { class ALCColorCorrectionWidget; }

class ALCColorCorrection;

class ALCColorCorrectionWidget : public QWidget {
  Q_OBJECT
private:
  Ui::ALCColorCorrectionWidget *ui;
  ALCColorCorrection *m_correction;

public:
  explicit ALCColorCorrectionWidget(QWidget *parent = 0);
  ~ALCColorCorrectionWidget();

  void setColorCorrection(ALCColorCorrection *);
  void reload();
  void restore();

private:
  void brightnessSliderChanged(int);
  void blueSliderChanged(int);
  void greenSliderChanged(int);
  void redSliderChanged(int);

  void setDefaultBrightness();
  void setDefaultRed();
  void setDefaultGreen();
  void setDefaultBlue();

};

#endif // ALCCOLORCORRECTIONWIDGET_H
