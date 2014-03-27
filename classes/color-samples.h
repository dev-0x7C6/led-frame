#ifndef COLORSAMPLES_H
#define COLORSAMPLES_H

#include <QVector>
#include <QDebug>


const int SAMPLE_RESOLUTION = 64;

class ColorSamples {
public:
  enum Position : quint8 {
    SAMPLE_BOTTOM,
    SAMPLE_LEFT,
    SAMPLE_TOP,
    SAMPLE_RIGHT,
    SAMPLE_ARRAY
  };

private:
  QVector < int> m_samples[SAMPLE_ARRAY];
  quint16 m_sampleCount;

public:
  explicit ColorSamples();
  virtual ~ColorSamples();

  void set(Position pos, QVector < int> &colors);
  QVector < int> *get(Position pos);
  QVector < int>* scaled(Position pos, int size);
  QVector < int>* pscaled(Position pos, int size);

  quint16 scale() { return SAMPLE_RESOLUTION; }

  void copy(ColorSamples &ref);
};

#endif // COLORSAMPLES_H
