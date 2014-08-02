#ifndef ALCWEATHERCOLORCORRECTION_H
#define ALCWEATHERCOLORCORRECTION_H

#include <QObject>
#include <QNetworkRequest>
#include "classes/alc-color-correction.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class QTimerEvent;

class ALCWeatherColorCorrection : public QObject, public ALCColorCorrection {
  Q_OBJECT
private:
  static QString link;
  QNetworkAccessManager m_manager;
  QTime m_sunrise;
  QTime m_sunset;
  QTime m_midday;

public:
  explicit ALCWeatherColorCorrection(QObject *parent = 0);

  void fetchWeatherStatus();
  void callCorrection(const QTime &current);

  static ALCWeatherColorCorrection *instance();

protected:
  virtual void timerEvent(QTimerEvent *event);

private:
  void recieveReply(QNetworkReply *reply);

};

#endif // ALCWEATHERCOLORCORRECTION_H
