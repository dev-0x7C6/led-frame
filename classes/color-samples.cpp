#include "color-samples.h"

ColorSamples::ColorSamples() {
  m_samples[0].resize(SAMPLE_RESOLUTION);
  m_samples[1].resize(SAMPLE_RESOLUTION);
  m_samples[2].resize(SAMPLE_RESOLUTION);
  m_samples[3].resize(SAMPLE_RESOLUTION);
}

ColorSamples::~ColorSamples() { }

void ColorSamples::set(ColorSamples::Position pos, QVector < int> &colors) {
  memcpy(reinterpret_cast < void*> (m_samples[pos].data()),
         reinterpret_cast < void*> ( colors.data()), colors.size() * sizeof(int));
}

QVector < int> *ColorSamples::get(ColorSamples::Position pos) {
  return &m_samples[pos];
}

QVector < int> *ColorSamples::scaled(ColorSamples::Position pos, int size) {
  register QVector < int> *result = new QVector < int> (size);
  register const QVector < int> &samples = m_samples[pos];

  double step = samples.size() / static_cast < double>( size);
  for (register int i = 0; i < size; ++i)
    (*result)[i] = samples[i*step];

  (*result)[0] = samples.first();
  (*result)[size - 1] = samples.last();

  return result;
}

QVector < int> *ColorSamples::pscaled(ColorSamples::Position pos, int size) {
  register QVector < int> *result = new QVector < int> (size);
  register const QVector < int> &samples = m_samples[pos];

  double step = samples.size() / static_cast < double>( size);
}

//  int r = 0;
//  int g = 0;
//  int b = 0;

//  for (register int i = 0; i < samples.count(); ++i) {
//    r += qRed(samples[i]);
//    g += qGreen(samples[i]);
//    b += qBlue(samples[i]);
//    double d = 0;
//    for (double d = 0; d < step; d += (step/5.0))


//  }


//  double value = 0;
//   for (register int i = 0; i < size; ++i) {
//     for (register double d = 0; d < 1.0; d += step)
//       v = samples[i*step];
//  }

//  for (register int i = 0; i < size; ++i)
//    if ()
//    (*result)[i] = samples[i*step];
//  }

//  (*result)[0] = samples.first();
//  (*result)[size - 1] = samples.last();

  //qDebug() << (*result)

  //return result;

