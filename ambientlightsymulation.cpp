#include "ambientlightsymulation.h"

#include <QPainter>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQmlEngine>

#include <QPalette>
#include <QQuickView>
#include <QBoxLayout>
#include <QOpenGLContext>
#include <QSurfaceFormat>

ALCSymulation::ALCSymulation(QWidget *parent) :
  QWidget(parent),
  m_timerId(0),
  m_framerateLimit(30),
  m_emitter(0),
  m_glowSize(60)
{
  QPalette p = palette();
  p.setBrush(QPalette::Window, Qt::black);
  setPalette(p);
  setUpdatesEnabled(false);
  setAutoFillBackground(true);
  m_monitor = QPixmap(":/256x256/display.png");
  setFramerate(24);
  startTimer(30);

  QQmlEngine *engine = new QQmlEngine;

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);


  QSurfaceFormat format(QSurfaceFormat::StereoBuffers);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setRenderableType(QSurfaceFormat::OpenGLES);
  format.setSwapInterval(1000.0/24.0);
//  format.setStencilBufferSize(2 * 1024 * 1024);
 // format.setDepthBufferSize(2 * 1024 * 1024);

  QQuickView *view = new QQuickView();
 // view->setFormat(format);

  QWidget *container = QWidget::createWindowContainer(view, this);
  container->setMinimumSize(500, 350);
  container->setMaximumSize(500, 350);
  container->setFocusPolicy(Qt::TabFocus);
  view->setSource(QUrl("qrc:/qml/main.qml"));
  layout->addWidget(container);
  setLayout(layout);


  int s = 280;
  QQmlComponent comp(view->engine(), QUrl("qrc:/qml/LedAmbient.qml"));
  view->setPersistentOpenGLContext(true);


  QQuickItem *item;

  QRect draw(130, 100, 220, 150);

  for (register int ii = 0; ii < 4; ++ii) {
    for (register int i = 0; i < 8; ++i) {
      switch (ii) {
      case 2:
        item = qobject_cast<QQuickItem*>(comp.create());
        item->setX(draw.x() + (draw.width() / 8 * i) - s/2);
        item->setY(draw.y() - s/2);
        item->setWidth(s);
        item->setHeight(s);
        item->setParent(view->rootObject());
        item->setParentItem(qobject_cast<QQuickItem *>(view->rootObject()));
        break;
      case 0:
        item = qobject_cast<QQuickItem*>(comp.create());
        item->setX(draw.x() + (draw.width() / 8 * (8-i)) - s/2);
        item->setY(draw.y() + draw.height() - s/2);
        item->setWidth(s);
        item->setHeight(s);
        item->setParent(view->rootObject());
        item->setParentItem(qobject_cast<QQuickItem *>(view->rootObject()));
        break;
      case 1:
        item = qobject_cast<QQuickItem*>(comp.create());
        item->setX(draw.x() - s/2);
        item->setY(draw.y() + draw.height() / 8 * (8-i) - s/2);
        item->setWidth(s);
        item->setHeight(s);
        item->setParent(view->rootObject());
        item->setParentItem(qobject_cast<QQuickItem *>(view->rootObject()));
        break;
      case 3:
        item = qobject_cast<QQuickItem*>(comp.create());
        item->setX(draw.x() + draw.width() - s/2);
        item->setY(draw.y() + draw.height() / 8 * i - s/2);
        item->setWidth(s);
        item->setHeight(s);
        item->setParent(view->rootObject());
        item->setParentItem(qobject_cast<QQuickItem *>(view->rootObject()));
        break;
      }
      quick[ii][i] = item;
      item->setAntialiasing(false);
      item->setSmooth(false);
    }
  }

  QQmlComponent comp2(view->engine(), QUrl("qrc:/qml/Monitor.qml"));
  item = qobject_cast<QQuickItem*>(comp2.create());
  item->setX(view->width() / 2 - 128);
  item->setY(view->height() / 2 - 106);
  item->setZ(1000);
  item->setParent(view->rootObject());
  item->setParentItem(qobject_cast<QQuickItem *>(view->rootObject()));
}

void ALCSymulation::setFramerate(int value) {
  if (m_timerId)
    killTimer(m_timerId);
  m_timerId = startTimer(1000.0/(m_framerateLimit = value));
}

void ALCSymulation::setGlowSize(int value) {
  m_glowSize = value;
}

#include "classes/alc-color-samples.h"
#include <QElapsedTimer>

#include <QGuiApplication>

void ALCSymulation::timerEvent(QTimerEvent *) {


  if (m_emitter) {
    m_emitter->state(m_samples);

    for (register int ii = 0; ii < 4; ++ii) {
      QVector < int> *colors = m_samples.scaled(ALCColorSamples::Position(ii), 8);
      for (register int i = 0; i < 8; ++i)  {
        QColor color((*colors)[i]);
        QQuickItem *item = quick[ii][i];
        if (item->property("sample").toString() != color.name()) {
          quint16 s = (color.red() + color.green() + color.blue())/3;
          quick[ii][i]->setOpacity(1.0 - (s/255.0) * 0.5);
          quick[ii][i]->setZ(255 - color.black());
          quick[ii][i]->setProperty("sample", QColor((*colors)[i]).name());
        }
      }
      delete colors;
    }
  }
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


void ALCSymulation::paintEvent(QPaintEvent *) {

  return;

  /*

  int x = ((width() - m_monitor.width())/2) + 9;
  int y = ((height() - m_monitor.height())/2) + 9;

  QVector <int> *colors = m_samples.scaled(ALCColorSamples::SAMPLE_TOP, 8);

  for (register int i = 0; i < colors->size(); ++i)
    drawLedAmbient(i * ((m_monitor.width())/8.0) + x, y, m_glowSize, QColor((*colors)[i]), painter);

  colors = m_samples.scaled(ALCColorSamples::SAMPLE_RIGHT, 8);

  for (register int i = 0; i < colors->size(); ++i)
    drawLedAmbient(width() - x, i * ((m_monitor.height())/27*2)+y+15, m_glowSize, QColor ((*colors)[i]), painter);

  colors = m_samples.scaled(ALCColorSamples::SAMPLE_BOTTOM, 8);

  for (register int i = 0; i < colors->size(); ++i)
    drawLedAmbient(i * ((m_monitor.width())/8.0) + x, height() - y - (m_monitor.height()/4.0), m_glowSize, QColor((*colors)[i]), painter);

  colors = m_samples.scaled(ALCColorSamples::SAMPLE_LEFT, 8);

  for (register int i = 0; i < colors->size(); ++i)
    drawLedAmbient(x, i * ((m_monitor.height())/27*2)+y+15, m_glowSize, QColor ((*colors)[i]), painter);
*/


}
