#include "blackhole-color-emitter.h"

BlackholeColorEmitter::BlackholeColorEmitter() :
  ColorEmitter()
{
  m_type = EMITTER_BLACKHOLE;
  QList < QRgb> colors;
  for (register int i = 0; i < 64; ++i)
    colors << 0;

//  setState(colors);
}

BlackholeColorEmitter::~BlackholeColorEmitter() {}
