#ifndef ANIMATIONCOLOREMITTER_H
#define ANIMATIONCOLOREMITTER_H

#include <QObject>
#include <QColor>
#include <QRgb>

#include <QPropertyAnimation>
#include <QDebug>


class AnimationColorEmitter : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)

private:
  QPropertyAnimation *m_animation;
  QColor m_color;
  QList < QRgb> m_colors;

  QColor color() { return m_color; }
  void setColor(QColor value) {
    m_color = value;


   // qDebug() << value;
  }



public:
  explicit AnimationColorEmitter(QObject *parent = 0) :
    QObject(parent),
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
    m_animation->setDuration(320*4);
    m_animation->setKeyValueAt(0.000, QColor::fromRgbF(1, 1, 0));
    m_animation->setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
    m_animation->setKeyValueAt(0.666, QColor::fromRgbF(1, 0, 1));
    m_animation->setKeyValueAt(1.000, QColor::fromRgbF(1, 1, 0));
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
