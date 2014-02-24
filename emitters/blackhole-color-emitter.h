#ifndef BLACKHOLECOLOREMITTER_H
#define BLACKHOLECOLOREMITTER_H

#include <QObject>
#include "emitters/color-emitter.h"

class BlackholeColorEmitter : public ColorEmitter {
public:
  explicit BlackholeColorEmitter();
  virtual ~BlackholeColorEmitter();
};

#endif // BLACKHOLECOLOREMITTER_H
