#ifndef ALCEMITTERSWIDGET_H
#define ALCEMITTERSWIDGET_H

#include <QMainWindow>
#include <QPushButton>

class ColorEmitter;

class QPushButtonEx :public QPushButton {
private:
  ColorEmitter *m_emitter;

public:
  explicit QPushButtonEx(QWidget *parent = 0) :
    QPushButton(parent),
    m_emitter(0) {}

  void setEmitter(ColorEmitter *emitter) {
    m_emitter = emitter;
  }

  ColorEmitter *emitter() {
    return m_emitter;
  }
};


namespace Ui { class ALCEmittersWidget; }

class ALCEmittersWidget : public QMainWindow {
  Q_OBJECT
private:
  Ui::ALCEmittersWidget *ui;

public:
  explicit ALCEmittersWidget(QWidget *parent = 0);
  ~ALCEmittersWidget();

  void addPlainColorItem();
  void addAnimationItem();
  void addImageItem();

  void prepare();
  void setup();

private:
  void insertPlainColorItem(ColorEmitter *);
  void insertAnimationItem(ColorEmitter *);
  void insertImageItem(ColorEmitter *);

  void pickAnimation();
  void pickImage();
  void pickColor();

  void prepareColorButton(QPushButton *, QColor);


};

#endif // ALCEMITTERSWIDGET_H
