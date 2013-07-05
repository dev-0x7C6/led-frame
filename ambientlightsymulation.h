#ifndef AMBIENTLIGHTSYMULATION_H
#define AMBIENTLIGHTSYMULATION_H

#include "serialbackend.h"

#include <QWidget>
#include <QRgb>

class AmbientLightSymulation :public QWidget
{
  Q_OBJECT
private:
  QList< QRgb> colors;
  QPixmap m_monitor;
  SerialBackend m_backend;

  int m_timerId;
  int m_framerateLimit;
  int m_glowSize;

public:
  explicit AmbientLightSymulation(QWidget *parent = 0);

public slots:
  void updateLeds(QList< QRgb> c);
  void setFramerate(int);
  void setGlowSize(int);


protected:
  void timerEvent(QTimerEvent *);

private slots:

protected:
  void paintEvent(QPaintEvent *);
};

#endif // AMBIENTLIGHTSYMULATION_H
