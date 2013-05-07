#include "ambientlightsymulation.h"

#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>

AmbientLightSymulation::AmbientLightSymulation(QWidget *parent) : QWidget(parent)
{
}

void AmbientLightSymulation::updateLeds(QList< QRgb> c) {
  colors = c;
  update(0, 0, width(), height());
}


void drawLedAmbient(qreal x, qreal y, qreal radius, QColor color, QPainter &painter) {
  QRadialGradient grad(x, y, radius);
  color.setAlpha(255 - (color.black()/1));
  QColor a = color;
  a.setAlpha(a.alpha()/3);
  grad.setColorAt(1.0, QColor::fromRgbF(0, 0, 0, 0));
  grad.setColorAt(0.5, a);
  grad.setColorAt(0.0, color);
  painter.setBrush(grad);
  painter.drawEllipse(x - radius, y - radius, radius*2, radius*2);
}

void AmbientLightSymulation::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(0, 0, width(), height());

  if (colors.isEmpty())
    return;

  int colorid = 0;
  for (register int i = 0; i < 8; ++i) {
    QColor color(colors[colorid++]);
    drawLedAmbient(i * ((width()-96)/7)+48, 64, 96, color, painter);
  }

  for (register int i = 0; i < 8; ++i) {
    QColor color(colors[colorid++]);
    drawLedAmbient(i * ((width()-96)/7)+48, 210, 96, color, painter);
  }

  for (register int i = 0; i < 8; ++i) {
    QColor color(colors[colorid++]);
    drawLedAmbient(48, i * ((width()-190)/8)+96, 96, color, painter);
  }

  for (register int i = 0; i < 8; ++i) {
    QColor color(colors[colorid++]);
    drawLedAmbient(width() - 48, i * ((width()-190)/8)+96, 96, color, painter);
  }
}
