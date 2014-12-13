#ifndef ALCWEATHERCOLORCORRECTION_H
#define ALCWEATHERCOLORCORRECTION_H

#include "classes/alc-color-correction.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTimer>

class ALCWeatherColorCorrection : public QObject, public ALCColorCorrection {
  Q_OBJECT
private:
  static const quint32 fetchTimeout;
  static QString link;
  QNetworkAccessManager m_manager;
  QTimer m_timer;
  QTime m_sunrise;
  QTime m_sunset;
  QTime m_midday;

public:
  explicit ALCWeatherColorCorrection(QObject *parent = 0);
  static ALCWeatherColorCorrection *instance();

protected:
  void fetchWeatherStatus();
  void callCorrection(const QTime &current);

private:
  void recieveReply(QNetworkReply *reply);

};

#endif // ALCWEATHERCOLORCORRECTION_H
