/**********************************************************************************
 * AmbientLedDriver - https://gitorious.org/ambientleddriver -                    *
 * Copyright (C) 2014  Bartłomiej Burdukiewicz                                    *
 * Contact: bartlomiej.burdukiewicz@gmail.com                                     *
 *                                                                                *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU Lesser General Public                     *
 * License as published by the Free Software Foundation; either                   *
 * version 2.1 of the License, or (at your option) any later version.             *
 *                                                                                *
 * This program is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU              *
 * Lesser General Public License for more details.                                *
 *                                                                                *
 * You should have received a copy of the GNU Lesser General Public               *
 * License along with this program; if not, see <http://www.gnu.org/licences/>.   *
 **********************************************************************************/

#include "alc-emitter-manager.h"

#include "classes/alc-color-correction.h"
#include "classes/alc-settings.h"
#include "managers/alc-device-manager.h"
#include "connector/alc-device-thread.h"
#include "emitters/alc-animation-emitter.h"
#include "emitters/alc-color-emitter.h"
#include "emitters/alc-image-emitter.h"
#include "emitters/alc-screen-emitter.h"
#include "widgets/alc-symulation-widget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QSettings>

ALCEmitterManager::ALCEmitterManager(QObject *parent) :
  QObject(parent) {
  QSettings *settings = ALCSettings::instance()->settings();
  settings->beginGroup("emitters");
  settings->beginGroup("screens");
  QList < QScreen* > screens = QApplication::screens();

  for (register int i = -1; i < QApplication::desktop()->screenCount(); ++i) {
    ALCScreenEmitter *emitter = new ALCScreenEmitter(0);
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
    emitter->setMarginProcent(settings->value("clip", 0).toDouble());
    readColorCorrection(settings, dynamic_cast < ALCColorCorrection*>(emitter));
    emitter->start();
    settings->endGroup();
    m_emitters[ALCEmitter::EMITTER_SCREEN_CAPTURE] << emitter;
  }

  settings->endGroup();

  if (settings->childGroups().contains("animations")) {
    settings->beginGroup("animations");
    QStringList list = settings->childGroups();

    for (register int i = 0; i < list.count(); ++i) {
      settings->beginGroup(list[i]);
      addALCAnimationEmitter(settings->value("name", QString()).toString());
      settings->endGroup();
    }

    settings->endGroup();
  } else {
    for (register int i = 0; i < 3; ++i) {
      addALCAnimationEmitter("Animation #" + QString::number(i + 1));
    }
  }

  if (settings->childGroups().contains("colors")) {
    settings->beginGroup("colors");
    QStringList list = settings->childGroups();

    for (register int i = 0; i < list.count(); ++i) {
      settings->beginGroup(list[i]);
      ALCColorEmitter *emitter = addALCColorEmitter(settings->value("name", QString()).toString());
      int r = settings->value("red", 0).toInt();
      int g = settings->value("green", 0).toInt();
      int b = settings->value("blue", 0).toInt();
      emitter->setColor(QColor::fromRgb(r, g, b));
      settings->endGroup();
    }

    settings->endGroup();
  } else {
    for (register int i = 0; i < 3; ++i)
      addALCColorEmitter("Plain color #" + QString::number(i + 1));
  }

  if (settings->childGroups().contains("images")) {
    settings->beginGroup("images");
    QStringList list = settings->childGroups();

    for (register int i = 0; i < list.count(); ++i) {
      settings->beginGroup(list[i]);
      ALCImageEmitter *emitter = addALCImageEmitter(settings->value("name", QString()).toString());
      emitter->fromFile(settings->value("file", QString()).toString());
      settings->endGroup();
    }

    settings->endGroup();
  } else {
    for (register int i = 0; i < 3; ++i)
      addALCImageEmitter("Image samples #" + QString::number(i + 1));
  }

  settings->endGroup();
}

ALCEmitterManager::~ALCEmitterManager() {
  QSettings *settings = ALCSettings::instance()->settings();
  settings->beginGroup("emitters");
  QList < ALCEmitter *> all = allEmitters();

  for (register int i = 0; i < all.count(); ++i) {
    switch (all[i]->type()) {
      case ALCEmitter::EMITTER_SCREEN_CAPTURE:
        dynamic_cast < ALCScreenEmitter*>(all[i])->setQuitState(true);
        break;

      default:
        break;
    }
  }

  settings->remove("animations");
  settings->remove("colors");
  settings->remove("images");
  ALCAnimationEmitter *animation;
  ALCImageEmitter *image;
  ALCColorEmitter *plain;
  ALCScreenEmitter *screen;

  for (register int i = 0; i < all.count(); ++i) {
    switch (all[i]->type()) {
      case ALCEmitter::EMITTER_SCREEN_CAPTURE:
        settings->beginGroup("screens");
        screen = dynamic_cast < ALCScreenEmitter*>(all[i]);
        screen->wait();
        settings->beginGroup(screen->emitterName());
        settings->setValue("name", screen->emitterName());
        settings->setValue("pixelSkip", screen->pixelSkip());
        settings->setValue("chunk", screen->chunk());
        settings->setValue("fps", screen->framerateLimit());
        settings->setValue("clip", screen->marginProcent());
        writeColorCorrection(settings, dynamic_cast < ALCColorCorrection*>(screen));
        settings->endGroup();
        settings->endGroup();
        break;

      case ALCEmitter::EMITTER_ANIMATION:
        animation = dynamic_cast < ALCAnimationEmitter*>(all[i]);
        settings->beginGroup("animations");
        settings->beginGroup(QString::number(i));
        settings->setValue("name", animation->emitterName());
        settings->setValue("remember", true);
        settings->endGroup();
        settings->endGroup();
        break;

      case ALCEmitter::EMITTER_PLAIN_COLOR:
        plain = dynamic_cast < ALCColorEmitter*>(all[i]);
        settings->beginGroup("colors");
        settings->beginGroup(QString::number(i));
        settings->setValue("name", plain->emitterName());
        settings->setValue("red", plain->color().red());
        settings->setValue("green", plain->color().green());
        settings->setValue("blue", plain->color().blue());
        settings->endGroup();
        settings->endGroup();
        break;

      case ALCEmitter::EMITTER_IMAGE:
        image = dynamic_cast < ALCImageEmitter*>(all[i]);
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

void ALCEmitterManager::add(ALCEmitter *emitter, ALCEmitter::EmitterType type) {
  m_emitters[type] << emitter;
  emit emitterListChanged();
}

ALCImageEmitter *ALCEmitterManager::addALCImageEmitter(const QString &name) {
  ALCImageEmitter *emitter = new ALCImageEmitter(this);
  emitter->setEmitterName(name);
  add(emitter, ALCEmitter::EMITTER_IMAGE);
  return emitter;
}

ALCColorEmitter *ALCEmitterManager::addALCColorEmitter(const QString &name) {
  ALCColorEmitter *emitter = new ALCColorEmitter();
  emitter->setEmitterName(name);
  add(emitter, ALCEmitter::EMITTER_PLAIN_COLOR);
  return emitter;
}

ALCAnimationEmitter *ALCEmitterManager::addALCAnimationEmitter(const QString &name) {
  ALCAnimationEmitter *emitter = new ALCAnimationEmitter();
  emitter->setEmitterName(name);
  add(emitter, ALCEmitter::EMITTER_ANIMATION);
  return emitter;
}

ALCScreenEmitter *ALCEmitterManager::addScreenCaptureEmitter(const QString &name) {
  ALCScreenEmitter *emitter = new ALCScreenEmitter(this);
  emitter->setEmitterName(name);
  add(emitter, ALCEmitter::EMITTER_SCREEN_CAPTURE);
  return emitter;
}

void ALCEmitterManager::remove(ALCEmitter *emitter) {
  for (register int i = 0; i < ALCDeviceManager::instance()->count(); ++i) {
    if (ALCDeviceManager::instance()->device(i)->connectedEmitter() == emitter)
      ALCDeviceManager::instance()->device(i)->connectEmitter(0);

    if (m_symulation->connectedEmitter() == emitter)
      m_symulation->connectEmitter(0);
  }

  for (register int i = 0; i < ALCEmitter::EMITTER_END_ARRAY; ++i)
    m_emitters[static_cast< ALCEmitter::EmitterType>(i)].removeAll(emitter);

  delete emitter;
  emit emitterListChanged();
}

void ALCEmitterManager::addSymulation(ALCSymulationWidget *symulation) {
  m_symulation = symulation;
}

QList < ALCEmitter *> *ALCEmitterManager::emitters(ALCEmitter::EmitterType type) {
  return &m_emitters[type];
}

QList<ALCEmitter *> ALCEmitterManager::allEmitters() {
  QList < ALCEmitter *> result;

  for (register int i = 0; i < ALCEmitter::EMITTER_END_ARRAY; ++i)
    result << m_emitters[static_cast< ALCEmitter::EmitterType>(i)];

  return result;
}

void ALCEmitterManager::readColorCorrection(QSettings *settings, ALCColorCorrection *correction) {
  settings->beginGroup("correction");
  correction->setBrightness(settings->value("light").toDouble());
  correction->setRedCorrection(settings->value("red").toDouble());
  correction->setGreenCorrection(settings->value("green").toDouble());
  correction->setBlueCorrection(settings->value("blue").toDouble());
  settings->endGroup();
}

void ALCEmitterManager::writeColorCorrection(QSettings *settings, ALCColorCorrection *correction) {
  settings->beginGroup("correction");
  settings->setValue("light", correction->brightness());
  settings->setValue("red", correction->redCorrection());
  settings->setValue("green", correction->greenCorrection());
  settings->setValue("blue", correction->blueCorrection());
  settings->endGroup();
}

