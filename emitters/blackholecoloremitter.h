#ifndef BLACKHOLECOLOREMITTER_H
#define BLACKHOLECOLOREMITTER_H

#include <QObject>
#include "emitters/coloremitter.h"

class BlackholeColorEmitter : public QObject, public ColorEmitter
{
  Q_OBJECT
public:
  explicit BlackholeColorEmitter(QObject *parent = 0);  
};

#endif // BLACKHOLECOLOREMITTER_H
