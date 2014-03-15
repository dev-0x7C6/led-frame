#include "alc-screen-manager.h"

#include "emitters/screen-capture-color-emitter.h"
#include "classes/alc-settings.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QScreen>

ALCScreenManager::ALCScreenManager(QObject *parent) :
  QObject(parent)
{
  QSettings *settings = ALCSettings::instance()->settings();
  settings->beginGroup("ALCScreenManager");
  settings->beginGroup("Dynamic");

  QList < QScreen* > screens = QApplication::screens();


  for (register int i = -1; i < QApplication::desktop()->screenCount(); ++i) {
    ScreenCaptureColorEmitter *emitter = new ScreenCaptureColorEmitter(0);
    QRect rect;
    switch (i) {
    case -1:
      rect = QApplication::desktop()->geometry();
      emitter->setName("Visible area");
      break;
    default:
      rect = screens[i]->geometry();
      emitter->setName(screens[i]->name());
      break;
    }

    settings->beginGroup(emitter->name());
    emitter->setCaptureArea(rect);
    emitter->setPixelSkip(settings->value("PixelSkip", 4).toInt());
    emitter->setChunkSize(settings->value("Chunk", 250).toInt());
    emitter->setFramerateLimit(settings->value("FPS", 60).toInt());
    emitter->setBrightness(settings->value("Brightness", 1.0).toDouble());
    emitter->start();
    settings->endGroup();
    m_screenEmitters << emitter;
  }

  settings->endGroup();
  settings->endGroup();
}

ALCScreenManager::~ALCScreenManager() {
  QSettings *settings = ALCSettings::instance()->settings();
  settings->beginGroup("ALCScreenManager");
  settings->beginGroup("Dynamic");

  for (register int i = 0; i < m_screenEmitters.count(); ++i) {
    m_screenEmitters[i]->setQuitState(true);
  }

  for (register int i = 0; i < m_screenEmitters.count(); ++i) {
    ScreenCaptureColorEmitter *emitter = m_screenEmitters[i];
    emitter->wait();
    settings->beginGroup(emitter->name());
    settings->setValue("PixelSkip", emitter->pixelSkip());
    settings->setValue("Chunk", emitter->chunk());
    settings->setValue("FPS", emitter->framerateLimit());
    settings->setValue("Brightness", emitter->brightness());
    settings->endGroup();
    delete emitter;
  }

  settings->endGroup();
  settings->endGroup();

}

ScreenCaptureColorEmitter *ALCScreenManager::add() {
  ScreenCaptureColorEmitter *emitter = new ScreenCaptureColorEmitter(0);
  m_screenEmitters << emitter;
}

void ALCScreenManager::remove(ScreenCaptureColorEmitter *emitter) {
  m_screenEmitters.removeAll(emitter);
  delete emitter;
}

QList < ScreenCaptureColorEmitter *> *ALCScreenManager::emitters() {
  return &m_screenEmitters;
}
