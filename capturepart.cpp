#include "capturepart.h"

#include <QScreen>
#include <QPixmap>
#include <QImage>

QList <QRgb> grab(QScreen *screen, QRect &area, ScreenFragments fragment,
                      quint8 chunk, quint8 skip, double brightness) {
  register quint32 r;
  register quint32 g;
  register quint32 b;
  register qint32 length;

  QPixmap pixmap;
  QList < QRgb> list;

  switch (fragment) {
  case Top:
    pixmap = screen->grabWindow(0, area.x(), area.y(), area.width(), chunk);
    break;
  case Bottom:
    pixmap = screen->grabWindow(0, area.x(), area.y() + area.height() - chunk, area.width() - chunk);
    break;
  case Left:
    pixmap = screen->grabWindow(0, area.x(), area.y() + chunk, chunk, area.height() - chunk*2);
    break;
  case Right:
    pixmap = screen->grabWindow(0, area.x() + area.width() - chunk, area.y() + chunk, chunk, area.height() - chunk*2);
    break;
  }

  QImage image;

  for (register int i = 0; i < 8; ++i) {
    r = g = b = 0;

    switch (fragment) {
    case Top:
      image = pixmap.copy(i * area.width() / 8, 0, chunk, chunk).toImage();
      break;
    case Bottom:
      image = pixmap.copy((8-i) * area.width() / 8, 0, chunk, chunk).toImage();
      break;
    case Left:
      image = pixmap.copy(0, (8-i) * area.height() / 8, chunk, chunk).toImage();
      break;
    case Right:
      image = pixmap.copy(0, i * area.height() / 8, chunk, chunk).toImage();
      break;
    }

    quint32 *source = reinterpret_cast< quint32*>(image.bits());
    length = image.byteCount() / image.depth();

    for (register int i = 0; i < length; i += skip) {
      r += uchar(source[i] >> 8);
      g += uchar(source[i] >> 16);
      b += uchar(source[i]);
    }

    r = r / (length / (skip * brightness));
    g = g / (length / (skip * brightness));
    b = b / (length / (skip * brightness));
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    list << qRgb(r, g, b);
  }

  return list;
}

