#include "alc-led-strip-manager.h"

#include "classes/alc-led-strip-configuration.h"
#include "classes/alc-settings.h"

#include <QSettings>

ALCLedStripManager::ALCLedStripManager() {
  QSettings *settings = ALCSettings::instance()->settings();
  if (settings->childGroups().contains("led-strips")) {
    settings->beginGroup("led-strips");
    settings->endGroup();
  } else {
    settings->beginGroup("led-strips");
    settings->endGroup();
  }
}

ALCLedStripManager::~ALCLedStripManager() {

}

QList<ALCLedStripConfiguration *> ALCLedStripManager::configurations() {
  return m_configurations;
}
