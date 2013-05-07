#ifndef AMBIENTLIGHTSYMULATION_H
#define AMBIENTLIGHTSYMULATION_H

#include <QWidget>
#include <QRgb>

class AmbientLightSymulation :public QWidget
{
  Q_OBJECT
private:
  QList< QRgb> colors;


public:
  explicit AmbientLightSymulation(QWidget *parent = 0);

public slots:
  void updateLeds(QList< QRgb> c);

private slots:

protected:
  void paintEvent(QPaintEvent *);
};

#endif // AMBIENTLIGHTSYMULATION_H
