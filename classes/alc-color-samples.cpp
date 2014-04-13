#include "alc-color-samples.h"

ALCColorSamples::ALCColorSamples() {
  m_samples[0].resize(SAMPLE_RESOLUTION);
  m_samples[1].resize(SAMPLE_RESOLUTION);
  m_samples[2].resize(SAMPLE_RESOLUTION);
  m_samples[3].resize(SAMPLE_RESOLUTION);
}

ALCColorSamples::~ALCColorSamples() { }

void ALCColorSamples::set(ALCColorSamples::Position pos, QVector < int> &colors) {
  memcpy(reinterpret_cast < void*> (m_samples[pos].data()),
         reinterpret_cast < void*> ( colors.data()), colors.size() * sizeof(int));
}

QVector < int> *ALCColorSamples::get(ALCColorSamples::Position pos) {
  return &m_samples[pos];
}

QVector < int> *ALCColorSamples::scaled(ALCColorSamples::Position pos, int size) {
  register QVector < int> *result = new QVector < int> (size);
  register const QVector < int> &samples = m_samples[pos];

  double step = samples.size() / static_cast < double>( size);
  for (register int i = 0; i < size; ++i)
    (*result)[i] = samples[i*step];

  (*result)[0] = samples.first();
  (*result)[size - 1] = samples.last();

  return result;
}

QVector < int> *ALCColorSamples::pscaled(ALCColorSamples::Position pos, int size) {
  register QVector < int> *result = new QVector < int> (size);
  register const QVector < int> &samples = m_samples[pos];

  double step = samples.size() / static_cast < double>( size);
  Q_UNUSED(step);
  return result;
}

void ALCColorSamples::copy(ALCColorSamples &ref) {
  for (register int i = 0; i < SAMPLE_ARRAY; ++i)
    set(static_cast< Position>(i), ref.m_samples[i]);
}
