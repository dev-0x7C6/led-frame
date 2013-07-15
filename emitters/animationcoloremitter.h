#ifndef ANIMATIONCOLOREMITTER_H
#define ANIMATIONCOLOREMITTER_H

#include <QObject>
#include <QColor>
#include <QRgb>

#include <QPropertyAnimation>
#include <QDebug>

#include "emitters/coloremitter.h"


class AnimationColorEmitter : public QObject, public ColorEmitter
{
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)

private:
  QPropertyAnimation *m_animation;
  QColor m_color;

  QColor color() { return m_color; }
  void setColor(QColor value) {
    m_color = value;


   // qDebug() << value;
  }



public:
  explicit AnimationColorEmitter(QObject *parent = 0) :
    QObject(parent),
    ColorEmitter(),
    m_animation(0)
  {
    rotatePalette();
    startTimer(1000/24);
  }

private:
  void rotatePalette() {
    if (m_animation)
      delete m_animation;

    m_animation = new QPropertyAnimation(this, "color");
    m_animation->setDuration(320*4*10);
//    m_animation->setKeyValueAt(0.000, QColor::fromRgbF(1, 1, 0));
//    m_animation->setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
//    m_animation->setKeyValueAt(0.666, QColor::fromRgbF(1, 0, 1));
//    m_animation->setKeyValueAt(1.000, QColor::fromRgbF(1, 1, 0));

    m_animation->setKeyValueAt(0.0, QColor::fromRgbF(1, 1, 0));
    m_animation->setKeyValueAt(0.1, QColor::fromRgbF(0, 1, 0));
    m_animation->setKeyValueAt(0.2, QColor::fromRgbF(0, 1, 1));
    m_animation->setKeyValueAt(0.3, QColor::fromRgbF(0, 0, 1));
    m_animation->setKeyValueAt(0.4, QColor::fromRgbF(1, 1, 0));
    m_animation->setKeyValueAt(0.5, QColor::fromRgbF(1, 0, 1));
    m_animation->setKeyValueAt(0.6, QColor::fromRgbF(0, 0, 0));
    m_animation->setKeyValueAt(0.7, QColor::fromRgbF(0, 1, 0));
    m_animation->setKeyValueAt(0.8, QColor::fromRgbF(0, 1, 1));
    m_animation->setKeyValueAt(0.9, QColor::fromRgbF(1, 1, 0));
    m_animation->setKeyValueAt(1.0, QColor::fromRgbF(0, 1, 1));

    m_animation->setLoopCount(-1);
    m_animation->start();
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
  void timerEvent(QTimerEvent *) {
    if (!m_connectedCount)
      return;

    if (m_colors.count() == 32)
      m_colors.removeFirst();
    m_colors << m_color.rgb();

    if (m_colors.count() == 32)
    emit updateLeds(m_colors);
  }
public slots:
signals:
  void updateLeds(QList <QRgb> c);
};

#endif // ANIMATIONCOLOREMITTER_H
