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
#include "classes/alc-settings.h"
#include "connector/alc-device-thread.h"
#include "emitters/alc-animation-emitter.h"
#include "emitters/alc-color-emitter.h"
#include "emitters/alc-image-emitter.h"
#include "emitters/alc-screen-emitter.h"
#include "managers/alc-device-manager.h"
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
	QList <QScreen *> screens = QApplication::screens();

	for (int i = -1; i < QApplication::desktop()->screenCount(); ++i) {
		Emitters::ALCScreenEmitter *emitter = new Emitters::ALCScreenEmitter(0);
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
		settings->endGroup();
		m_emitters[static_cast<int>(Emitters::ALCEmitter::Type::ScreenCapture)] << emitter;
	}

	settings->endGroup();

	if (settings->childGroups().contains("animations")) {
		settings->beginGroup("animations");
		QStringList list = settings->childGroups();

		for (int i = 0; i < list.count(); ++i) {
			settings->beginGroup(list[i]);
			addALCAnimationEmitter(settings->value("name", QString()).toString());
			settings->endGroup();
		}

		settings->endGroup();
	} else {
		for (int i = 0; i < 3; ++i)
			addALCAnimationEmitter("Animation #" + QString::number(i + 1));
	}

	if (settings->childGroups().contains("colors")) {
		settings->beginGroup("colors");
		QStringList list = settings->childGroups();

		for (int i = 0; i < list.count(); ++i) {
			settings->beginGroup(list[i]);
			Emitters::ALCColorEmitter *emitter = addALCColorEmitter(settings->value("name", QString()).toString());
			int r = settings->value("red", 0).toInt();
			int g = settings->value("green", 0).toInt();
			int b = settings->value("blue", 0).toInt();
			emitter->setColor(QColor::fromRgb(r, g, b));
			settings->endGroup();
		}

		settings->endGroup();
	} else {
		for (int i = 0; i < 3; ++i)
			addALCColorEmitter("Plain color #" + QString::number(i + 1));
	}

	if (settings->childGroups().contains("images")) {
		settings->beginGroup("images");
		QStringList list = settings->childGroups();

		for (int i = 0; i < list.count(); ++i) {
			settings->beginGroup(list[i]);
			Emitters::ALCImageEmitter *emitter = addALCImageEmitter(settings->value("name", QString()).toString());
			emitter->fromFile(settings->value("file", QString()).toString());
			settings->endGroup();
		}

		settings->endGroup();
	} else {
		for (int i = 0; i < 3; ++i)
			addALCImageEmitter("Image samples #" + QString::number(i + 1));
	}

	settings->endGroup();
}

ALCEmitterManager::~ALCEmitterManager() {
}

void ALCEmitterManager::done() {
	QSettings *settings = ALCSettings::instance()->settings();
	settings->beginGroup("emitters");
	QList <Emitters::ALCEmitter *> all = allEmitters();

	for (int i = 0; i < all.count(); ++i) {
		switch (all[i]->type()) {
			case Emitters::ALCEmitter::Type::ScreenCapture:
				dynamic_cast <Emitters::ALCScreenEmitter *>(all[i])->setQuitState(true);
				break;

			case Emitters::ALCEmitter::Type::Animation:
				dynamic_cast <Emitters::ALCAnimationEmitter *>(all[i])->setQuitState(true);
				break;

			default:
				break;
		}
	}

	settings->remove("animations");
	settings->remove("colors");
	settings->remove("images");
	Emitters::ALCAnimationEmitter *animation;
	Emitters::ALCImageEmitter *image;
	Emitters::ALCColorEmitter *plain;
	Emitters::ALCScreenEmitter *screen;

	for (int i = 0; i < all.count(); ++i) {
		switch (all[i]->type()) {
			case Emitters::ALCEmitter::Type::ScreenCapture:
				settings->beginGroup("screens");
				screen = dynamic_cast <Emitters::ALCScreenEmitter *>(all[i]);
				screen->wait();
				settings->beginGroup(screen->emitterName());
				settings->setValue("name", screen->emitterName());
				settings->setValue("pixelSkip", screen->pixelSkip());
				settings->setValue("chunk", screen->chunk());
				settings->setValue("fps", screen->framerateLimit());
				settings->setValue("clip", screen->marginProcent());
				settings->endGroup();
				settings->endGroup();
				break;

			case Emitters::ALCEmitter::Type::Animation:
				animation = dynamic_cast <Emitters::ALCAnimationEmitter *>(all[i]);
				animation->wait();
				settings->beginGroup("animations");
				settings->beginGroup(QString::number(i));
				settings->setValue("name", animation->emitterName());
				settings->setValue("remember", true);
				settings->endGroup();
				settings->endGroup();
				break;

			case Emitters::ALCEmitter::Type::PlainColor:
				plain = dynamic_cast <Emitters::ALCColorEmitter *>(all[i]);
				settings->beginGroup("colors");
				settings->beginGroup(QString::number(i));
				settings->setValue("name", plain->emitterName());
				settings->setValue("red", plain->color().red());
				settings->setValue("green", plain->color().green());
				settings->setValue("blue", plain->color().blue());
				settings->endGroup();
				settings->endGroup();
				break;

			case Emitters::ALCEmitter::Type::Image:
				image = dynamic_cast <Emitters::ALCImageEmitter *>(all[i]);
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

void ALCEmitterManager::add(Emitters::ALCEmitter *emitter, Emitters::ALCEmitter::Type type) {
	m_emitters[static_cast<int>(type)] << emitter;
	emit emitterListChanged();
}

Emitters::ALCImageEmitter *ALCEmitterManager::addALCImageEmitter(const QString &name) {
	Emitters::ALCImageEmitter *emitter = new Emitters::ALCImageEmitter(this);
	emitter->setEmitterName(name);
	add(emitter, Emitters::ALCEmitter::Type::Image);
	return emitter;
}

Emitters::ALCColorEmitter *ALCEmitterManager::addALCColorEmitter(const QString &name) {
	Emitters::ALCColorEmitter *emitter = new Emitters::ALCColorEmitter();
	emitter->setEmitterName(name);
	add(emitter, Emitters::ALCEmitter::Type::PlainColor);
	return emitter;
}

Emitters::ALCAnimationEmitter *ALCEmitterManager::addALCAnimationEmitter(const QString &name) {
	Emitters::ALCAnimationEmitter *emitter = new Emitters::ALCAnimationEmitter();
	emitter->setEmitterName(name);
	add(emitter, Emitters::ALCEmitter::Type::Animation);
	return emitter;
}

Emitters::ALCScreenEmitter *ALCEmitterManager::addScreenCaptureEmitter(const QString &name) {
	Emitters::ALCScreenEmitter *emitter = new Emitters::ALCScreenEmitter(this);
	emitter->setEmitterName(name);
	add(emitter, Emitters::ALCEmitter::Type::ScreenCapture);
	return emitter;
}

void ALCEmitterManager::remove(Emitters::ALCEmitter *) {
	//  for (int i = 0; i < ALCDeviceManager::instance()->count(); ++i) {
	//    if (ALCDeviceManager::instance()->device(i)->connectedEmitter() == emitter)
	//      ALCDeviceManager::instance()->device(i)->connectEmitter(0);
	//    if (m_symulation->connectedEmitter() == emitter)
	//      m_symulation->connectEmitter(0);
	//  }
	//  for (int i = 0; i < static_cast<int>(Emitters::ALCEmitter::Type::Last); ++i)
	//    m_emitters[i].removeAll(emitter);
	//  delete emitter;
	//  emit emitterListChanged();
}

ALCEmitterManager *ALCEmitterManager::instance() {
	static ALCEmitterManager object;
	return &object;
}

void ALCEmitterManager::addSymulation(ALCSymulationWidget *symulation) {
	m_symulation = symulation;
}

const QList <Emitters::ALCEmitter *> *ALCEmitterManager::emitters(Emitters::ALCEmitter::Type type) {
	return &m_emitters[static_cast<int>(type)];
}

const QList<Emitters::ALCEmitter *> ALCEmitterManager::allEmitters() {
	QList <Emitters::ALCEmitter *> result;

	for (int i = 0; i < static_cast<int>(Emitters::ALCEmitter::Type::Last); ++i)
		result << m_emitters[i];

	return result;
}

//void ALCEmitterManager::readColorCorrection(QSettings *, Correctors::ALCColorCorrection *) {
////  settings->beginGroup("correction");
////  correction->setCorrection(Correctors::ALCColorCorrection::Color::Brightness, settings->value("light").toDouble());
////  correction->setCorrection(Correctors::ALCColorCorrection::Color::Red, settings->value("red").toDouble());
////  correction->setCorrection(Correctors::ALCColorCorrection::Color::Green, settings->value("green").toDouble());
////  correction->setCorrection(Correctors::ALCColorCorrection::Color::Blue, settings->value("blue").toDouble());
////  settings->endGroup();
//}

//void ALCEmitterManager::writeColorCorrection(QSettings *, Correctors::ALCColorCorrection *) {
////  settings->beginGroup("correction");
////  settings->setValue("light", correction->correction(Correctors::ALCColorCorrection::Color::Brightness));
////  settings->setValue("red", correction->correction(Correctors::ALCColorCorrection::Color::Red));
////  settings->setValue("green", correction->correction(Correctors::ALCColorCorrection::Color::Green));
////  settings->setValue("blue", correction->correction(Correctors::ALCColorCorrection::Color::Blue));
////  settings->endGroup();
//}

