#include "plain-color-emitter.h"

#include <QTimer>

#include <QTime>
PlainColorEmitter::PlainColorEmitter() :
  QObject(),
  ColorEmitter(),
  m_timer(new QTimer(this))
{
  srand(QTime::currentTime().msecsSinceStartOfDay());
  m_type = EMITTER_PLAIN_COLOR;
  m_color = QColor::fromRgb(qrand()%255, qrand()%255, qrand()%255);


  connect(m_timer, &QTimer::timeout, this, &PlainColorEmitter::pushState);
  m_timer->setInterval(20);
  m_timer->start();
}

PlainColorEmitter::~PlainColorEmitter() {}

double max(double value) {
  if (value > 255)
    return 255;
  return value;
}

void PlainColorEmitter::pushState() {
  QVector < int> samples(SAMPLE_RESOLUTION);

  int rgb = qRgb(max(m_color.red() * m_brightness),
                 max(m_color.green() * m_brightness),
                 max(m_color.blue() * m_brightness));

  for (register int i = 0; i < samples.size(); ++i)
    samples[i] = rgb;

  m_samples.set(ColorSamples::SAMPLE_TOP, samples);
  m_samples.set(ColorSamples::SAMPLE_LEFT, samples);
  m_samples.set(ColorSamples::SAMPLE_RIGHT, samples);
  m_samples.set(ColorSamples::SAMPLE_BOTTOM, samples);
  setState(m_samples);
}
