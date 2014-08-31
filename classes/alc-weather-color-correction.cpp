#include "alc-weather-color-correction.h"

#include <QDomDocument>
#include <QTimer>

const quint32 ALCWeatherColorCorrection::fetchTimeout = 5000; // update after 30sec

QString ALCWeatherColorCorrection::link = QString("http://weather.yahooapis.com/forecastrss?");

ALCWeatherColorCorrection::ALCWeatherColorCorrection(QObject *parent) :
  QObject(parent),
  ALCColorCorrection() {
  m_type = WEATHER;
  connect(&m_manager, &QNetworkAccessManager::finished, this, &ALCWeatherColorCorrection::recieveReply);
  fetchWeatherStatus();

  m_timer.setSingleShot(true);
  m_timer.setInterval(fetchTimeout);
  connect(&m_timer, &QTimer::timeout, this, &ALCWeatherColorCorrection::fetchWeatherStatus);
}

ALCWeatherColorCorrection *ALCWeatherColorCorrection::instance() {
  static ALCWeatherColorCorrection object;
  return &object;
}

void ALCWeatherColorCorrection::fetchWeatherStatus() {
  QNetworkRequest request(QUrl("http://weather.yahooapis.com/forecastrss?w=526363&u=c"));
  m_manager.get(request);
}


void ALCWeatherColorCorrection::recieveReply(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError) {
    QDomDocument document;
    document.setContent(reply->readAll());
    QDomElement channel = document.firstChildElement("rss").firstChildElement("channel");
    m_sunrise = QTime::fromString(channel.firstChildElement("yweather:astronomy").attribute("sunrise"), "hh:mm ap");
    m_sunset = QTime::fromString(channel.firstChildElement("yweather:astronomy").attribute("sunset"), "hh:mm ap");
    m_midday = QTime::fromMSecsSinceStartOfDay(m_sunset.msecsSinceStartOfDay() - m_sunrise.msecsSinceStartOfDay());
    callCorrection(QTime::currentTime());
  } else {
    clear();
  }

  m_timer.start();

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

