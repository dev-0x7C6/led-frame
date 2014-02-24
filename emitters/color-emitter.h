#ifndef COLOREMITTER_H
#define COLOREMITTER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QDebug>

#include "classes/color-samples.h"

class ColorEmitter {
public:
  enum EmitterType: quint8 {
    EMITTER_NOT_DEFINED,
    EMITTER_BLACKHOLE,
    EMITTER_ANIMATION,
    EMITTER_PLAIN_COLOR,
    EMITTER_IMAGE,
    EMITTER_SCREEN_CAPTURE
  };

private:
  QList <QRgb> m_colors;
  ColorSamples m_samples;

protected:
  EmitterType m_type;
  QMutex m_mutex;
  int m_connectedCount;
  double m_brightness;

public:
  explicit ColorEmitter();
  virtual ~ColorEmitter();

  EmitterType type() const;
  void setType(const EmitterType type) { m_type = type; }

  void setBrightness(double brightness);
  double brightness();
  virtual void init();
  void done();
//  void setState(QList < QRgb> colors);
  void setState(ColorSamples &samples);
  void state(ColorSamples &samples);

  //QList < QRgb> state();
};

#endif // COLOREMITTER_H
