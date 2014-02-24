#ifndef AMBIENTLIGHTSYMULATION_H
#define AMBIENTLIGHTSYMULATION_H

#include "emitters/color-emitter.h"

#include <QWidget>
#include <QRgb>

class AmbientLightSymulation :public QWidget
{
  Q_OBJECT
private:
  QList< QRgb> colors;
  QPixmap m_monitor;
  ColorEmitter *m_emitter;

  int m_timerId;
  int m_framerateLimit;
  int m_glowSize;

public:
  explicit AmbientLightSymulation(QWidget *parent = 0);
  ~AmbientLightSymulation() {
    if (m_emitter)
      m_emitter->done();
  }

public slots:
  void setFramerate(int);
  void setGlowSize(int);

  void connectEmitter(ColorEmitter *emitter) {
    if (m_emitter)
      m_emitter->done();

    (m_emitter = emitter)->init();
  }

protected:
  void timerEvent(QTimerEvent *);

private slots:

protected:
  void paintEvent(QPaintEvent *);
};

#endif // AMBIENTLIGHTSYMULATION_H
