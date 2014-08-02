#include "alc-weather-color-correction.h"

#include <QDomDocument>
#include <QTimerEvent>
#include <QDebug>

QString ALCWeatherColorCorrection::link = QString("http://weather.yahooapis.com/forecastrss?");

ALCWeatherColorCorrection::ALCWeatherColorCorrection(QObject *parent) :
  QObject(parent),
  ALCColorCorrection() {
  m_type = WEATHER;
  connect(&m_manager, &QNetworkAccessManager::finished, this, &ALCWeatherColorCorrection::recieveReply);
  fetchWeatherStatus();
  startTimer(30000);
}

void ALCWeatherColorCorrection::fetchWeatherStatus() {
  QNetworkRequest request(QUrl("http://weather.yahooapis.com/forecastrss?w=526363&u=c"));
  m_manager.get(request);
}

ALCWeatherColorCorrection *ALCWeatherColorCorrection::instance() {
  static ALCWeatherColorCorrection object;
  return &object;
}

void ALCWeatherColorCorrection::timerEvent(QTimerEvent *event) {
  Q_UNUSED(event)
  fetchWeatherStatus();
}

void ALCWeatherColorCorrection::recieveReply(QNetworkReply *reply) {
  QDomDocument document;
  document.setContent(reply->readAll());
  QDomElement channel = document.firstChildElement("rss").firstChildElement("channel");
  m_sunrise = QTime::fromString(channel.firstChildElement("yweather:astronomy").attribute("sunrise"), "hh:mm ap");
  m_sunset = QTime::fromString(channel.firstChildElement("yweather:astronomy").attribute("sunset"), "hh:mm ap");
  m_midday = QTime::fromMSecsSinceStartOfDay(m_sunset.msecsSinceStartOfDay() - m_sunrise.msecsSinceStartOfDay());
  callCorrection(QTime::currentTime());
}

void ALCWeatherColorCorrection::callCorrection(const QTime &current) {
  double clock;
  double pm = m_sunset.msecsSinceStartOfDay() - m_midday.msecsSinceStartOfDay();
  double am = m_midday.msecsSinceStartOfDay() - m_sunrise.msecsSinceStartOfDay();
  pm /= 1.5;
  am /= 1.5;

  if (current >= m_midday)
    clock = pm / (current.msecsSinceStartOfDay() - m_midday.msecsSinceStartOfDay());
  else
    clock = am / (m_midday.msecsSinceStartOfDay() - current.msecsSinceStartOfDay());

  clock = qMin(clock, 3.0);
  setCorrection(ALCColorCorrection::Brightness, clock);
  qDebug() << "Current time:" << current << ", global: "  << correction(ALCColorCorrection::Brightness, true) << "scalar: " << clock;
}

