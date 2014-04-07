#include "emitters/animation-color-emitter.h"

AnimationColorEmitter::AnimationColorEmitter() :
  QObject(),
  ColorEmitter(),
  m_animation(0)
{
  m_type = EMITTER_ANIMATION;
  rotatePalette();
  startTimer(1000/30);
  for (register int i = 0; i < 256; ++i)
    m_colorStream << qRgb(0, 0, 0);

  m_animationType = Glow;
}

AnimationColorEmitter::~AnimationColorEmitter() {
  if (m_animation)
    delete m_animation;
}

#include <QMessageBox>

bool AnimationColorEmitter::open(){
  QMessageBox::warning(0, "Warning", "To implement.", QMessageBox::Ok);
}

void AnimationColorEmitter::rotatePalette() {
  if (m_animation)
    delete m_animation;

  m_animation = new QPropertyAnimation(this, "color");
  m_animation->setDuration(3200);
  m_animation->setKeyValueAt(0.000, QColor::fromRgbF(1, 0, 0));
  m_animation->setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
  m_animation->setKeyValueAt(0.666, QColor::fromRgbF(0, 0, 1));
  m_animation->setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
  m_animation->setLoopCount(-1);
  m_animation->start();
  ;
}

void AnimationColorEmitter::glow() {
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

unsigned char AnimationColorEmitter::max(int value) {
  if (value > 255)
    return 255; else
    return value;
}

void AnimationColorEmitter::timerEvent(QTimerEvent *) {
  if (!m_connectedCount)
    return;

  m_blink++;
  int rgb;

  switch (m_animationType) {
  case Rotation:
  {
    if (m_colorStream.count() == 256)
      m_colorStream.removeFirst();

    m_colorStream << qRgb(max(qRed(m_color.rgb())),
                          max(qGreen(m_color.rgb())),
                          max(qBlue(m_color.rgb())));

    QListIterator < QRgb> it(m_colorStream);
    for (register int p = 0; p < 4; ++p) {
      QVector < int> *vec = m_samples.get(static_cast < ALCColorSamples::Position>( p));
      for (register int i = 0; i < SAMPLE_RESOLUTION; ++i)
        (*vec)[i] = it.next();
    }
  }
    break;

  case Glow:
    if (m_blink < 2)
      rgb = qRgb(max(qRed(m_color.rgb())), max(qGreen(m_color.rgb())), max(qBlue(m_color.rgb()))); else
      rgb = qRgb(0,0,0);
    if (m_blink > 3)
      m_blink = 0;
    for (register int p = 0; p < 4; ++p) {
      QVector < int> *vec = m_samples.get(static_cast < ALCColorSamples::Position>( p));
      for (register int i = 0; i < SAMPLE_RESOLUTION; ++i)
        (*vec)[i] = rgb;
    }
    break;
  }
  setState(m_samples);
}
