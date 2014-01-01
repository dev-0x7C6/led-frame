#include "blackholecoloremitter.h"

BlackholeColorEmitter::BlackholeColorEmitter(QObject *parent) :
  QObject(parent),
  ColorEmitter()
{
  QList < QRgb> colors;
  for (register int i = 0; i < 64; ++i)
    colors << 0;

  setState(colors);
}
