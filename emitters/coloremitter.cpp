#include "coloremitter.h"

ColorEmitter::ColorEmitter(QObject *parent) :
  QObject(parent)
{
  m_connectedCount = 0;
  for (register int i = 0; i < 32; ++i)
    m_colors << 0;
}
