#ifndef IMAGECOLOREMITTER_H
#define IMAGECOLOREMITTER_H

#include <QObject>
#include <QImage>
#include <QTimer>

#include "emitters/color-emitter.h"

class ImageColorEmitter : public QObject, public ColorEmitter {
  Q_OBJECT
private:
  ColorSamples m_samples;
  QImage *m_image;
  QTimer *m_timer;

public:
  explicit ImageColorEmitter(QObject *parent = 0);
  

  void fromImage(QImage *image);
  void pushState();

  void init();

public slots:
  void open();
  
};

#endif // IMAGECOLOREMITTER_H
