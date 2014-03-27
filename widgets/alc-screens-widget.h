#ifndef ALCSCREENSWIDGET_H
#define ALCSCREENSWIDGET_H

#include <QMainWindow>
#include <QPushButton>

class ColorEmitter;

namespace Ui { class ALCScreensWidget; }

class ALCScreensWidget : public QMainWindow {
  Q_OBJECT
private:
  Ui::ALCScreensWidget *ui;

public:
  explicit ALCScreensWidget(QWidget *parent = 0);
  ~ALCScreensWidget();
  void setup();

private:
  void insertScreenCaptureItem(ColorEmitter *);
  void configure();

//  void prepareColorButton(QPushButton *, QColor);

};

#endif // ALCSCREENSWIDGET_H
