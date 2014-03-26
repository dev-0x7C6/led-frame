#ifndef COLOREMITTER_H
#define COLOREMITTER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QDebug>

#include "classes/color-samples.h"

class QTreeWidgetItem;


class ColorEmitter {
public:
  enum EmitterType: quint8 {
    EMITTER_NOT_DEFINED = 0x00,
    EMITTER_SCREEN_CAPTURE,
    EMITTER_BLACKHOLE,
    EMITTER_ANIMATION,
    EMITTER_PLAIN_COLOR,
    EMITTER_IMAGE,
    EMITTER_END_ARRAY
  };

private:
  QString m_emitterName;
  ColorSamples m_samples;
  QTreeWidgetItem *m_treeItem;

protected:
  EmitterType m_type;
  QMutex m_mutex;
  int m_connectedCount;
  double m_brightness;

public:
  explicit ColorEmitter();
  virtual ~ColorEmitter();

  void setEmitterName(const QString &name);
  QString emitterName() const;


  EmitterType type() const;
  void setType(const EmitterType type) { m_type = type; }

  void setBrightness(double brightness);
  double brightness();
  virtual void init();
  void done();
//  void setState(QList < QRgb> colors);
  void setState(ColorSamples &samples);
  void state(ColorSamples &samples);


  void setTreeItem(QTreeWidgetItem *item);
  QTreeWidgetItem *treeItem();


  //QList < QRgb> state();
};

#endif // COLOREMITTER_H
