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

#include "alc-led-strip-manager.h"

#include "classes/alc-strip-configuration.h"
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

const QList<ALCStripConfiguration *> &ALCLedStripManager::configurations() {
	return m_configurations;
}

ALCLedStripManager *ALCLedStripManager::instance() {
	static ALCLedStripManager object;
	return &object;
}
