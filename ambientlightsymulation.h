#ifndef AMBIENTLIGHTSYMULATION_H
#define AMBIENTLIGHTSYMULATION_H

#include "emitters/color-emitter.h"


#include <QWidget>
#include <QRgb>

#include "classes/alc-color-samples.h"

#include <QQuickItem>
#include <QVector>

class ALCSymulation :public QWidget
{
  Q_OBJECT
private:
  QPixmap m_monitor;
  ColorEmitter *m_emitter;
  ALCColorSamples m_samples;
  QQuickItem *quick[4][16];

  int z;

  int m_timerId;
  int m_framerateLimit;
  int m_glowSize;

public:
  explicit ALCSymulation(QWidget *parent = 0);
  ~ALCSymulation() {
    if (m_emitter)
      m_emitter->done();
    z = 100000;
  }

public slots:
  void setFramerate(int);
  void setGlowSize(int);

  void connectEmitter(ColorEmitter *emitter) {
    if (m_emitter)
      m_emitter->done();
    m_emitter = emitter;
    if (m_emitter)
      m_emitter->init();
  }


private slots:

protected:
  void paintEvent(QPaintEvent *);
  void timerEvent(QTimerEvent *);
};

#endif // AMBIENTLIGHTSYMULATION_H
