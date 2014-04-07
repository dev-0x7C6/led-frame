#ifndef PLAINCOLOREMITTER_H
#define PLAINCOLOREMITTER_H

#include <QColor>
#include <QObject>
#include "emitters/color-emitter.h"
#include "classes/alc-color-samples.h"

class QTimer;

class PlainColorEmitter: public QObject, public ColorEmitter {
  Q_OBJECT
private:
  QColor m_color;
  QTimer *m_timer;
  ALCColorSamples m_samples;

public:
  explicit PlainColorEmitter();
  virtual ~PlainColorEmitter();

  void setColor(QColor);
  QColor color();

  void pushState();

  QColor open();
};

#endif // PLAINCOLOREMITTER_H
