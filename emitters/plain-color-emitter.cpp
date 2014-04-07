#include "plain-color-emitter.h"

#include <QTimer>
#include <QTime>

PlainColorEmitter::PlainColorEmitter() :
  QObject(),
  ColorEmitter(),
  m_timer(new QTimer(this)),
  m_color(Qt::white)
{
  srand(QTime::currentTime().msecsSinceStartOfDay());
  m_type = EMITTER_PLAIN_COLOR;

  connect(m_timer, &QTimer::timeout, this, &PlainColorEmitter::pushState);
  m_timer->setInterval(20);
  m_timer->start();
}

PlainColorEmitter::~PlainColorEmitter() {}

void PlainColorEmitter::setColor(QColor color) {
  m_color = color;
}

QColor PlainColorEmitter::color() {
  return m_color;
}

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

  m_samples.set(ALCColorSamples::SAMPLE_TOP, samples);
  m_samples.set(ALCColorSamples::SAMPLE_LEFT, samples);
  m_samples.set(ALCColorSamples::SAMPLE_RIGHT, samples);
  m_samples.set(ALCColorSamples::SAMPLE_BOTTOM, samples);
  setState(m_samples);
}

#include <QColorDialog>

QColor PlainColorEmitter::open() {
  QColor color = QColorDialog::getColor(m_color);
  return (m_color = color);
}
