#ifndef CAPTUREPART_H
#define CAPTUREPART_H

#include <QList>
#include <QRect>
#include <QRgb>

class QScreen;

enum ScreenFragments {
  Top = 0,
  Bottom,
  Left,
  Right
};

QList < QRgb> grab(QScreen *screen, QRect &area, ScreenFragments fragment, quint8 chunk, quint8 skip, double brightness);

#endif
