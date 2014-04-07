#ifndef ANIMATIONCOLOREMITTER_H
#define ANIMATIONCOLOREMITTER_H

#include <QObject>
#include <QColor>
#include <QRgb>

#include <QPropertyAnimation>

#include "emitters/color-emitter.h"

class AnimationColorEmitter :public QObject, public ColorEmitter {
  Q_OBJECT
  Q_PROPERTY(QColor color READ color WRITE setColor)
public:
  enum AnimationType : quint8 {
    Blink,
    Glow,
    Rotation
  };

private:
  QPropertyAnimation *m_animation;
  QColor m_color;

  QColor color() { return m_color; }
  void setColor(QColor value) {
    m_color = value;
  }

  QList < QRgb> m_colorStream;
  ALCColorSamples m_samples;
  AnimationType m_animationType;
  quint64 m_blink;

public:
  explicit AnimationColorEmitter();
  virtual ~AnimationColorEmitter();

  bool open();

private:
  void rotatePalette();
  void glow ();

protected:
  unsigned char max(int value);

  void timerEvent(QTimerEvent *);

};

#endif // ANIMATIONCOLOREMITTER_H
