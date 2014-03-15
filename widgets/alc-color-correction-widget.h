#ifndef ALCCOLORCORRECTIONWIDGET_H
#define ALCCOLORCORRECTIONWIDGET_H

#include <QWidget>

namespace Ui { class ALCColorCorrectionWidget; }

class ALCColorCorrectionWidget : public QWidget {
  Q_OBJECT
private:
  Ui::ALCColorCorrectionWidget *ui;

public:
  explicit ALCColorCorrectionWidget(QWidget *parent = 0);
  ~ALCColorCorrectionWidget();

  void fromGlobal();

private:
  void brightnessSliderChanged(int);
  void blueSliderChanged(int);
  void greenSliderChanged(int);
  void redSliderChanged(int);

};

#endif // ALCCOLORCORRECTIONWIDGET_H
