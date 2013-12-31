#include "blackholecoloremitter.h"

BlackholeColorEmitter::BlackholeColorEmitter(QObject *parent) :
  ColorEmitter(),
  QObject(parent)
{
  QList < QRgb> colors;
  for (register int i = 0; i < 32; ++i)
    colors << 0;

  setState(colors);
}
