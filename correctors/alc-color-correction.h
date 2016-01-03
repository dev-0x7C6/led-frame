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

#ifndef ALCCOLORCORRECTION_H
#define ALCCOLORCORRECTION_H

#include <QObject>
#include <QList>
#include <atomic>

class QSettings;

namespace Correctors {
	struct ALCColorCorrectionValues;

	class ALCColorCorrection {
	public:
		enum class Type {
			Global,
			Device,
			User
		};

		enum class Color {
			Brightness,
			Red,
			Green,
			Blue,
			Last
		};

		enum class Format {
			RGB,
			RBG,
			GRB,
			BRG,
			GBR,
			BGR
		};

	public:
		explicit ALCColorCorrection(ALCColorCorrection::Type type);

		void setFormat(const Format format);
		Format format();

		void setCorrection(ALCColorCorrection::Color color, double value);
		double correction(ALCColorCorrection::Color color, bool global = false);

		void setCorrectionValues(const ALCColorCorrectionValues &values);
		ALCColorCorrectionValues correctionValues(bool global = false);

		void multiplyCorrectionValues(ALCColorCorrectionValues &values);

		void clear();

		bool enabled();
		void setEnabled(bool enabled);

		void saveCorrection(QSettings *);
		void loadCorrection(QSettings *);

	protected:
		std::atomic <double> m_r;
		std::atomic <double> m_g;
		std::atomic <double> m_b;
		std::atomic <double> m_l;
		std::atomic <bool> m_enabled;
		std::atomic <Format> m_format;
		std::atomic <Type> m_type;
	};

}

#endif // ALCCOLORCORRECTION_H
