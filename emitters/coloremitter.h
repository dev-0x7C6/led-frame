#ifndef COLOREMITTER_H
#define COLOREMITTER_H

#include <QObject>

class ColorEmitter : public QObject
{
  Q_OBJECT
public:
  explicit ColorEmitter(QObject *parent = 0);
  
protected:
  void timerEvent(QTimerEvent *) {}
  
public slots:
  
};

#endif // COLOREMITTER_H
