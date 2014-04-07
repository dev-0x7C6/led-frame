#include "alc-led-strip-configuration.h"

ALCLedStripConfiguration::ALCLedStripConfiguration(QObject *parent) :
  QObject(parent)
{
}

void ALCLedStripConfiguration::add(ALCLedStrip::Source source, ALCLedStrip::Destination destination,
                                   int count, bool clockwise, Format color, double brightness) {
  ALCLedStrip *strip = new ALCLedStrip();
  strip->setSource(source);
  strip->setDestination(destination);
  strip->setCount(count);
  strip->setClockwise(clockwise);
  strip->setColorFormat(color);
  strip->setBrightness(brightness);
  m_strips << strip;
}

QList<ALCLedStrip *> ALCLedStripConfiguration::list() {
  return m_strips;
}

void ALCLedStrip::setClockwise(bool clockwise) {
  m_clockwise = clockwise;
}

bool ALCLedStrip::clockwise() const{
  return m_clockwise;
}

void ALCLedStrip::setCount(int count) {
  m_count = count;
}

int ALCLedStrip::count() const {
  return m_count;
}

void ALCLedStrip::setColorFormat(Format format) {
  m_colorFormat = format;
}

Format ALCLedStrip::colorFormat() const {
  return m_colorFormat;
}

void ALCLedStrip::setSource(ALCLedStrip::Source src) {
  m_source = src;
}

ALCLedStrip::Source ALCLedStrip::source() const {
  return m_source;
}

void ALCLedStrip::setDestination(ALCLedStrip::Destination dest) {
  m_destination = dest;
}

ALCLedStrip::Destination ALCLedStrip::destination() const{
  return m_destination;
}

void ALCLedStrip::setBrightness(double value) {
  m_brightness = value;
}

double ALCLedStrip::brightness() {
  return m_brightness;
}
