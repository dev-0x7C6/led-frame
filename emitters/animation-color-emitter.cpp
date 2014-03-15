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

  m_animationType = Rotation;
}

AnimationColorEmitter::~AnimationColorEmitter() {
  if (m_animation)
    delete m_animation;
}
