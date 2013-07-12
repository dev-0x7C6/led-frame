#ifndef IMAGECOLOREMITTER_H
#define IMAGECOLOREMITTER_H

#include <QObject>


#include "emitters/coloremitter.h"

class imagecoloremitter : public QObject, public ColorEmitter
{
  Q_OBJECT
public:
  explicit imagecoloremitter(QObject *parent = 0);
  
signals:
  
public slots:
  
};

#endif // IMAGECOLOREMITTER_H
