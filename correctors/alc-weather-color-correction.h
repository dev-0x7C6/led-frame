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

#ifndef ALCWEATHERCOLORCORRECTION_H
#define ALCWEATHERCOLORCORRECTION_H

#include "correctors/alc-color-correction.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTimer>

namespace Correctors {

	class ALCWeatherColorCorrection : public QObject, public Correctors::ALCColorCorrection {
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

}

#endif // ALCWEATHERCOLORCORRECTION_H
