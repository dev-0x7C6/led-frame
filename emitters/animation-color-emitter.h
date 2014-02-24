#ifndef ANIMATIONCOLOREMITTER_H
#define ANIMATIONCOLOREMITTER_H

#include <QObject>
#include <QColor>
#include <QRgb>

#include <QPropertyAnimation>
#include <QDebug>

#include "emitters/color-emitter.h"

class AnimationColorEmitter :public QObject, public ColorEmitter {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)

private:
  QPropertyAnimation *m_animation;
  QColor m_color;

  QColor color() { return m_color; }
  void setColor(QColor value) {
    m_color = value;
  }

public:
  explicit AnimationColorEmitter();
  virtual ~AnimationColorEmitter();

private:
  void rotatePalette() {
    if (m_animation)
      delete m_animation;

    m_animation = new QPropertyAnimation(this, "color");
    m_animation->setDuration(320*6);
    m_animation->setKeyValueAt(0.000, QColor::fromRgbF(1, 1, 0));
    m_animation->setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 1));
    m_animation->setKeyValueAt(0.666, QColor::fromRgbF(1, 0, 1));
    m_animation->setKeyValueAt(1.000, QColor::fromRgbF(1, 1, 0));
    m_animation->setLoopCount(-1);
    m_animation->start();
   ;
  }

  void glow () {
    if (m_animation)
      delete m_animation;

    m_animation = new QPropertyAnimation(this, "color");
    m_animation->setDuration(10000);
    m_animation->setKeyValueAt(0.000, QColor::fromRgbF(0.0, 0.0, 0.0));
    m_animation->setKeyValueAt(0.500, QColor::fromRgbF(0.5, 1, 0));
    m_animation->setKeyValueAt(1.000, QColor::fromRgbF(0.0, 0.0, 0.0));
    m_animation->setLoopCount(-1);
    m_animation->start();
  }


protected:
  unsigned char max(int value) {
    if (value > 255)
      return 255; else
      return value;
  }

  void timerEvent(QTimerEvent *) {
    if (!m_connectedCount)
      return;

//    QList < QRgb> colors = state();
    double light = brightness();

//    if (colors.count() == 64)
//      colors.removeFirst();

//    colors <<  qRgb(max(qRed(m_color.rgb())*light),
//                     max(qGreen(m_color.rgb())*light),
//                     max(qBlue(m_color.rgb())*light));


  //  setState(colors);
//    if (colors.count() == 64)
//      emit updateLeds(colors);
  }
public slots:

};

#endif // ANIMATIONCOLOREMITTER_H
