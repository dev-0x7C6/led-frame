#ifndef SOLIDCOLOREMITTER_H
#define SOLIDCOLOREMITTER_H

#include <QObject>

class SolidColorEmitter : public QObject
{
  Q_OBJECT
public:
  explicit SolidColorEmitter(QObject *parent = 0);


protected:
  void timerEvent(QTimerEvent *) {}
  
signals:
  //void setLeds(QList <QRgb> colors);
};

#endif // SOLIDCOLOREMITTER_H
