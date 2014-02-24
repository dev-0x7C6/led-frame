#include "emitters/animation-color-emitter.h"

AnimationColorEmitter::AnimationColorEmitter() :
  QObject(),
  ColorEmitter(),
  m_animation(0)
{
  m_type = EMITTER_ANIMATION;
  rotatePalette();
  startTimer(1000/60);
}

AnimationColorEmitter::~AnimationColorEmitter() {
  if (m_animation)
    delete m_animation;
}
