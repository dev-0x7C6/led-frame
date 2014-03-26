#ifndef ALCLEDSTRIPMANAGER_H
#define ALCLEDSTRIPMANAGER_H

#include <QList>

class ALCLedStripConfiguration;

class ALCLedStripManager {
private:
  QList < ALCLedStripConfiguration*> m_configurations;

public:
  explicit ALCLedStripManager();
  virtual ~ALCLedStripManager();

  QList < ALCLedStripConfiguration*> configurations();


  static ALCLedStripManager* instance() {
    static ALCLedStripManager object;
    return &object;
  }

};

#endif // ALCLEDSTRIPMANAGER_H
