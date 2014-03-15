#ifndef ALCSETTINGS_H
#define ALCSETTINGS_H

#include <QObject>

class QSettings;

class ALCSettings : public QObject {
  Q_OBJECT
private:
  QSettings *m_settings;
public:
  explicit ALCSettings(QObject *parent = 0);
  virtual ~ALCSettings();

  QSettings *settings();

  void load();
  void save();

  static ALCSettings *instance() {
    static ALCSettings settings;
    return &settings;
  }

};

#endif // ALCSETTINGS_H
