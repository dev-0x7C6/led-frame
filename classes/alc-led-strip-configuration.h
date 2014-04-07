#ifndef ALCLEDSTRIPCONFIGURATION_H
#define ALCLEDSTRIPCONFIGURATION_H

#include <QObject>
#include <QList>

#include "classes/alc-color-correction.h"

class ALCLedStrip {
public:
  enum Source {
    SourceBottom,
    SourceLeft,
    SourceTop,
    SourceRight
  };

  enum Destination {
    DestinationBottom,
    DestinationLeft,
    DestinationTop,
    DestinationRight
  };

private:
  int m_count;
  bool m_clockwise;
  Source m_source;
  Destination m_destination;
  Format m_colorFormat;
  double m_brightness;

public:
  void setClockwise(bool);
  bool clockwise() const;

  void setCount(int count);
  int count() const;

  void setColorFormat(Format format);
  Format colorFormat() const;

  void setSource(Source);
  Source source() const;

  void setDestination(Destination);
  Destination destination() const;


  void setBrightness(double value);
  double brightness();

};

class ALCLedStripConfiguration : public QObject {
  Q_OBJECT
private:
  QList < ALCLedStrip*> m_strips;

public:
  explicit ALCLedStripConfiguration(QObject *parent = 0);

  void add(ALCLedStrip::Source source, ALCLedStrip::Destination destination,
           int count, bool clockwise = true, Format color = RGB, double brightness = 1.0);


  QList < ALCLedStrip *> list();

};

#endif // ALCLEDSTRIPCONFIGURATION_H
