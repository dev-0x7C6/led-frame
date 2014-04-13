#include "widgets/alc-symulation-widget.h"

#include "classes/alc-color-samples.h"
#include <QBoxLayout>
#include <QElapsedTimer>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QPainter>
#include <QPalette>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>
#include <QRadialGradient>
#include <QSurfaceFormat>

ALCSymulationWidget::ALCSymulationWidget(QWidget *parent) :
  QWidget(parent),
  m_emitter(0),
  m_view(new QQuickView())
{
  QPalette p = palette();
  p.setBrush(QPalette::Window, QColor::fromRgb(20, 20, 20));
  setPalette(p);
  setAutoFillBackground(true);
  startTimer(1000/24);

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
  QWidget *container = QWidget::createWindowContainer(m_view, this);
  container->setMinimumSize(500, 350);
  container->setMaximumSize(500, 350);
  container->setFocusPolicy(Qt::TabFocus);
  m_view->setSource(QUrl("qrc:/qml/Scene.qml"));
  layout->addWidget(container);
  setLayout(layout);

  createQmlMonitor();
  createQmlObjects();
}

ALCSymulationWidget::~ALCSymulationWidget() {
  if (m_emitter)
    m_emitter->done();

  freeQmlMonitor();
  freeQmlObjects();
  delete m_view;
}

void ALCSymulationWidget::connectEmitter(ColorEmitter *emitter) {
  if (m_emitter)
    m_emitter->done();
  m_emitter = emitter;
  if (m_emitter)
    m_emitter->init(); else
    resetQmlObjects();
}

void ALCSymulationWidget::createQmlMonitor() {
  QQmlComponent monitor(m_view->engine(), QUrl("qrc:/qml/Monitor.qml"));
  m_monitor = qobject_cast< QQuickItem* >( monitor.create());
  m_monitor->setX(500 / 2 - 128);
  m_monitor->setY(350 / 2 - 106);
  m_monitor->setZ(1000);
  m_monitor->setParent(m_view->rootObject());
  m_monitor->setParentItem(qobject_cast< QQuickItem * >(m_view->rootObject()));
}

void ALCSymulationWidget::freeQmlMonitor() {
  delete m_monitor;
}

void ALCSymulationWidget::createQmlObjects(int size) {
  QRect draw(130, 100, 220, 150);

  QQmlComponent led(m_view->engine(), QUrl("qrc:/qml/LedAmbient.qml"));
  QQuickItem *item;
  QQuickItem *root = qobject_cast< QQuickItem * >(m_view->rootObject());

  for (register int ii = 0; ii < 4; ++ii) {
    for (register int i = 0; i < 8; ++i) {
      item = qobject_cast<QQuickItem*>(led.create());
      switch (ii) {
      case 2:
        item->setX(draw.x() + (draw.width() / 8 * i) - size / 2);
        item->setY(draw.y() - size / 2);
        break;
      case 0:
        item->setX(draw.x() + (draw.width() / 8 * (8 - i)) - size / 2);
        item->setY(draw.y() + draw.height() - size / 2);
        break;
      case 1:
        item->setX(draw.x() - size / 2);
        item->setY(draw.y() + draw.height() / 8 * (8 - i) - size / 2);
        break;
      case 3:
        item->setX(draw.x() + draw.width() - size / 2);
        item->setY(draw.y() + draw.height() / 8 * i - size / 2);
        break;
      }
      m_items[ii][i] = item;
      item->setAntialiasing(false);
      item->setHeight(size);
      item->setSmooth(false);
      item->setWidth(size);
      item->setParent(m_view->rootObject());
      item->setParentItem(root);
    }
  }
}

void ALCSymulationWidget::freeQmlObjects() {
  for (register int ii = 0; ii < 4; ++ii)
    for (register int i = 0; i < 8; ++i)
      delete m_items[ii][i];
}

void ALCSymulationWidget::resetQmlObjects() {
  for (register int ii = 0; ii < 4; ++ii)
    for (register int i = 0; i < 8; ++i) {
      QQuickItem *item = m_items[ii][i];
      item->setOpacity(0.0);
      item->setZ(0);
      item->setProperty("sample", "black");
    }
}

void ALCSymulationWidget::timerEvent(QTimerEvent *) {
  if (m_emitter) {
    m_emitter->state(m_samples);

    for (register int ii = 0; ii < 4; ++ii) {
      QVector < int> *colors = m_samples.scaled(ALCColorSamples::Position(ii), 8);
      for (register int i = 0; i < 8; ++i)  {
        QColor color((*colors)[i]);
        QQuickItem *item = m_items[ii][i];
        if (item->property("sample").toString() != color.name()) {
          quint16 s = (color.red() + color.green() + color.blue())/3;
          item->setOpacity(1.0 - (s/255.0) * 0.5);
          item->setZ(255 - color.black());
          item->setProperty("sample", QColor((*colors)[i]).name());
        }
      }
      delete colors;
    }
  }
}
