#ifndef IMAGECOLOREMITTER_H
#define IMAGECOLOREMITTER_H

#include <QObject>
#include <QImage>
#include <QTimer>

#include "emitters/color-emitter.h"

class ImageColorEmitter : public QObject, public ColorEmitter {
  Q_OBJECT
private:
  ALCColorSamples m_samples;
  QImage m_image;
  QString m_file;
  bool m_loaded;
  QTimer *m_timer;

public:
  explicit ImageColorEmitter(QObject *parent = 0);
  
  void fromFile(QString file);
  QString file();
  QString open();

  bool isLoaded();


  void pushState();
  void init();
};

#endif // IMAGECOLOREMITTER_H
