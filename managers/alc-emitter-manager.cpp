#include "alc-emitter-manager.h"

#include "classes/alc-settings.h"
#include "emitters/animation-color-emitter.h"
#include "emitters/image-color-emitter.h"
#include "emitters/plain-color-emitter.h"
#include "emitters/screen-capture-color-emitter.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QSettings>

ALCEmitterManager::ALCEmitterManager(QObject *parent) :
  QObject(parent)
{
  QSettings *settings = ALCSettings::instance()->settings();
  settings->beginGroup("emitters");
  settings->beginGroup("screens");

  QList < QScreen* > screens = QApplication::screens();


  for (register int i = -1; i < QApplication::desktop()->screenCount(); ++i) {
    ScreenCaptureColorEmitter *emitter = new ScreenCaptureColorEmitter(0);
    QRect rect;
    switch (i) {
    case -1:
      rect = QApplication::desktop()->geometry();
      emitter->setName("all");
      emitter->setEmitterName(QString("Visible area (%1x%2) x: %3, y: %4").arg(
                                QString::number(rect.width()),
                                QString::number(rect.height()),
                                QString::number(rect.x()),
                                QString::number(rect.y())));
      break;
    default:
      rect = screens[i]->geometry();
      emitter->setEmitterName(QString("%1 (%2x%3) x: %4, y: %5").arg(
                                screens[i]->name(),
                                QString::number(rect.width()),
                                QString::number(rect.height()),
                                QString::number(rect.x()),
                                QString::number(rect.y())));
      emitter->setName(screens[i]->name());
      break;
    }

    settings->beginGroup(emitter->emitterName());
    emitter->setCaptureArea(rect);
    emitter->setPixelSkip(settings->value("pixelSkip", 4).toInt());
    emitter->setChunkSize(settings->value("chunk", 250).toInt());
    emitter->setFramerateLimit(settings->value("fps", 60).toInt());
    emitter->setBrightness(settings->value("brightness", 1.0).toDouble());
    emitter->start();

    settings->endGroup();
    m_emitters[ColorEmitter::EMITTER_SCREEN_CAPTURE] << emitter;
  }

  settings->endGroup();



  if (settings->childGroups().contains("animations")) {
    settings->beginGroup("animations");
    QStringList list = settings->childGroups();
    for (register int i = 0; i < list.count(); ++i) {
      settings->beginGroup(list[i]);
      AnimationColorEmitter *emitter = addAnimationColorEmitter();
      emitter->setEmitterName(settings->value("name", QString()).toString());
      settings->endGroup();
    }
    settings->endGroup();
  } else {
    for (register int i = 0; i < 3; ++i) {
      addAnimationColorEmitter()->setEmitterName("Animation #" + QString::number(i+1));
    }
  }

  if (settings->childGroups().contains("colors")) {
    settings->beginGroup("colors");
    QStringList list = settings->childGroups();
    for (register int i = 0; i < list.count(); ++i) {
      settings->beginGroup(list[i]);
      PlainColorEmitter *emitter = addPlainColorEmitter();
      int r = settings->value("red", 0).toInt();
      int g = settings->value("green", 0).toInt();
      int b = settings->value("blue", 0).toInt();
      emitter->setEmitterName(settings->value("name", QString()).toString());
      emitter->setColor(QColor::fromRgb(r, g, b));
      settings->endGroup();
    }
    settings->endGroup();
  } else {
    for (register int i = 0; i < 3; ++i)
      addPlainColorEmitter()->setEmitterName("Plain color #" + QString::number(i+1));;
  }

  if (settings->childGroups().contains("images")) {
    settings->beginGroup("images");

    QStringList list = settings->childGroups();
    for (register int i = 0; i < list.count(); ++i) {
      settings->beginGroup(list[i]);
      ImageColorEmitter *emitter = addImageColorEmitter();
      emitter->setEmitterName(settings->value("name", QString()).toString());
      emitter->fromFile(settings->value("file", QString()).toString());
      settings->endGroup();
    }
    settings->endGroup();
  } else {
    for (register int i = 0; i < 3; ++i)
      addImageColorEmitter()->setEmitterName("Image samples #" + QString::number(i+1));;
  }


  settings->endGroup();
}

ALCEmitterManager::~ALCEmitterManager() {
  QSettings *settings = ALCSettings::instance()->settings();
  settings->beginGroup("emitters");

  QList < ColorEmitter *> all = allEmitters();

  for (register int i = 0; i < all.count(); ++i) {
    switch (all[i]->type()) {
    case ColorEmitter::EMITTER_SCREEN_CAPTURE:
      dynamic_cast < ScreenCaptureColorEmitter*> ( all[i])->setQuitState(true);
      break;
    default:
      break;
    }
  }

  settings->remove("animations");
  settings->remove("colors");
  settings->remove("images");

  AnimationColorEmitter *animation;
  ImageColorEmitter *image;
  PlainColorEmitter *plain;
  ScreenCaptureColorEmitter *screen;

  for (register int i = 0; i < all.count(); ++i) {
    switch (all[i]->type()) {
    case ColorEmitter::EMITTER_SCREEN_CAPTURE:
      settings->beginGroup("screens");
      screen = dynamic_cast < ScreenCaptureColorEmitter*> ( all[i]);
      screen->wait();
      settings->beginGroup(screen->emitterName());
      settings->setValue("name", screen->emitterName());
      settings->setValue("pixelSkip", screen->pixelSkip());
      settings->setValue("chunk", screen->chunk());
      settings->setValue("fps", screen->framerateLimit());
      settings->setValue("brightness", screen->brightness());
      settings->endGroup();
      settings->endGroup();
      break;
    case ColorEmitter::EMITTER_ANIMATION:
      animation = dynamic_cast < AnimationColorEmitter*> ( all[i]);
      settings->beginGroup("animations");
      settings->beginGroup(QString::number(i));
      settings->setValue("name", animation->emitterName());
      settings->setValue("remember", true);
      settings->endGroup();
      settings->endGroup();
      break;
    case ColorEmitter::EMITTER_PLAIN_COLOR:
      plain = dynamic_cast < PlainColorEmitter*> ( all[i]);
      settings->beginGroup("colors");
      settings->beginGroup(QString::number(i));
      settings->setValue("name", plain->emitterName());
      settings->setValue("red", plain->color().red());
      settings->setValue("green", plain->color().green());
      settings->setValue("blue", plain->color().blue());
      settings->endGroup();
      settings->endGroup();
      break;
    case ColorEmitter::EMITTER_IMAGE:
      image = dynamic_cast < ImageColorEmitter*> ( all[i]);
      settings->beginGroup("images");
      settings->beginGroup(QString::number(i));
      settings->setValue("name", image->emitterName());
      settings->setValue("remember", true);
      settings->setValue("file", image->file());
      settings->endGroup();
      settings->endGroup();
      break;

    default:
      break;
    }
    delete all[i];
  }

  settings->endGroup();
}

void ALCEmitterManager::add(ColorEmitter *emitter, ColorEmitter::EmitterType type) {
  m_emitters[type] << emitter;
  emit emitterListChanged();
}

ImageColorEmitter *ALCEmitterManager::addImageColorEmitter() {
  ImageColorEmitter *emitter = new ImageColorEmitter(this);
  add(emitter, ColorEmitter::EMITTER_IMAGE);
  return emitter;
}

PlainColorEmitter *ALCEmitterManager::addPlainColorEmitter() {
  PlainColorEmitter *emitter = new PlainColorEmitter();
  add(emitter, ColorEmitter::EMITTER_PLAIN_COLOR);
  return emitter;
}

AnimationColorEmitter *ALCEmitterManager::addAnimationColorEmitter() {
  AnimationColorEmitter *emitter = new AnimationColorEmitter();
  add(emitter, ColorEmitter::EMITTER_ANIMATION);
  return emitter;
}

ScreenCaptureColorEmitter *ALCEmitterManager::addScreenCaptureEmitter() {
  add(new ScreenCaptureColorEmitter(this), ColorEmitter::EMITTER_SCREEN_CAPTURE);
}

void ALCEmitterManager::remove(ColorEmitter *emitter) {
  for (register int i = 0; i < ColorEmitter::EMITTER_END_ARRAY; ++i)
    m_emitters[static_cast< ColorEmitter::EmitterType>(i)].removeAll(emitter);
  delete emitter;
}

QList < ColorEmitter *> *ALCEmitterManager::emitters(ColorEmitter::EmitterType type) {
  return &m_emitters[type];
}

QList<ColorEmitter *> ALCEmitterManager::allEmitters() {
  QList < ColorEmitter *> result;
  for (register int i = 0; i < ColorEmitter::EMITTER_END_ARRAY; ++i)
    result << m_emitters[static_cast< ColorEmitter::EmitterType>( i)];
  return result;
}
