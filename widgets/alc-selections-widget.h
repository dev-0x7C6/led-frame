#ifndef ALCSELECTIONSWIDGET_H
#define ALCSELECTIONSWIDGET_H

#include <QMainWindow>
#include <QPushButton>

class ColorEmitter;

namespace Ui { class ALCSelectionsWidget; }

class ALCSelectionsWidget : public QMainWindow {
  Q_OBJECT
private:
  Ui::ALCSelectionsWidget *ui;

public:
  explicit ALCSelectionsWidget(QWidget *parent = 0);
  ~ALCSelectionsWidget();

private:
  void setup();
  void insertScreenCaptureItem(ColorEmitter *);

//  void prepareColorButton(QPushButton *, QColor);

};

#endif // ALCSELECTIONSWIDGET_H
