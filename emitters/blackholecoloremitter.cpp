#include "blackholecoloremitter.h"

BlackholeColorEmitter::BlackholeColorEmitter(QObject *parent) :
  ColorEmitter(),
  QObject(parent)
{
  m_colors << 0x00 << 0x00;
}
