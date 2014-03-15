#include "ambientlightsymulation.h"

#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>

AmbientLightSymulation::AmbientLightSymulation(QWidget *parent) :
  QGLWidget(parent),
  m_timerId(0),
  m_framerateLimit(30),
  m_emitter(0),
  m_glowSize(96)
{
  setUpdatesEnabled(false);
  setAutoFillBackground(false);
  m_monitor = QPixmap(":/256x256/display.png");
  setFramerate(30);
  startTimer(13);
}

void AmbientLightSymulation::setFramerate(int value) {
  if (m_timerId)
    killTimer(m_timerId);
  m_timerId = startTimer(1000.0/(m_framerateLimit = value));
}

void AmbientLightSymulation::setGlowSize(int value) {
  m_glowSize = value;
}


//void AmbientLightSymulation::timerEvent(QTimerEvent *) {
//  setUpdatesEnabled(false);
////  if (m_emitter)
////    colors = m_emitter->state();
//  repaint();
//  setUpdatesEnabled(true);
//}

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
  return;
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(0, 0, width(), height());

  int x = ((width() - m_monitor.width())/2) + 9;
  int y = ((height() - m_monitor.height())/2) + 9;
  if (!colors.isEmpty()) {
    int colorid = 0;
    for (register int i = 0; i < 16; ++i)
      drawLedAmbient(i * ((m_monitor.width())/16.0) + x, y, m_glowSize, QColor(colors[colorid++]), painter);

    for (register int i = 0; i < 16; ++i)
      drawLedAmbient(width() - x, i * ((m_monitor.height())/27)+y+15, m_glowSize, QColor (colors[colorid++]), painter);

    for (register int i = 15; i >= 0; --i)
      drawLedAmbient(i * ((m_monitor.width())/16.0) + x, height() - y - (m_monitor.height()/4.0), m_glowSize, QColor(colors[colorid++]), painter);

    for (register int i = 15; i >= 0; --i)
      drawLedAmbient(x, i * ((m_monitor.height())/27)+y+15, m_glowSize, QColor (colors[colorid++]), painter);

  }

  painter.drawPixmap((width() - m_monitor.width())/2,
                     (height() - m_monitor.height())/2,
                     m_monitor.width(), m_monitor.height(),
                     m_monitor);


}
