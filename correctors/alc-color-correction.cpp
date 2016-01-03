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

#include "alc-color-correction.h"
#include "correctors/alc-color-correction-values.h"
#include "correctors/alc-color-correction-manager.h"

#include <QSettings>

namespace Correctors {

	ALCColorCorrection::ALCColorCorrection(ALCColorCorrection::Type type)
		: m_type(type) {
		clear();
		m_enabled = true;
	}

	bool ALCColorCorrection::enabled() {
		return m_enabled;
	}

	void ALCColorCorrection::setEnabled(bool enabled) {
		m_enabled = enabled;
	}

	void ALCColorCorrection::saveCorrection(QSettings *settings) {
		double value = {};
		settings->setValue("l", value = m_l);
		settings->setValue("r", value = m_r);
		settings->setValue("g", value = m_g);
		settings->setValue("b", value = m_b);
	}

	void ALCColorCorrection::loadCorrection(QSettings *settings) {
		m_l = settings->value("l", 1.0).toDouble();
		m_r = settings->value("r", 1.0).toDouble();
		m_g = settings->value("g", 1.0).toDouble();
		m_b = settings->value("b", 1.0).toDouble();
	}

	void ALCColorCorrection::setFormat(const ALCColorCorrection::Format format) {
		m_format = format;
	}

	ALCColorCorrection::Format ALCColorCorrection::format() {
		return static_cast<Format>(m_format);
	}

	void ALCColorCorrection::setCorrection(ALCColorCorrection::Color color, double value) {
		switch (color) {
			case Color::Red:
				m_r = value;
				break;

			case Color::Green:
				m_g = value;
				break;

			case Color::Blue:
				m_b = value;
				break;

			case Color::Brightness:
				m_l = value;
				break;

			case Color::Last:
				break;
		}
	}

	double ALCColorCorrection::correction(ALCColorCorrection::Color color, bool global) {
		double correction = 0;

		switch (color) {
			case Color::Red:
				correction = m_r;
				break;

			case Color::Green:
				correction = m_g;
				break;

			case Color::Blue:
				correction = m_b;
				break;

			case Color::Brightness:
				correction = m_l;
				break;

			case Color::Last:
				break;
		}

		if (global)
			correction *= ALCColorCorrectionManager::instance()->correction(color);

		return correction;
	}

	void ALCColorCorrection::setCorrectionValues(const ALCColorCorrectionValues &values) {
		m_r = values.r;
		m_g = values.g;
		m_b = values.b;
		m_l = values.l;
	}

	ALCColorCorrectionValues ALCColorCorrection::correctionValues(bool global) {
		ALCColorCorrectionValues values;
		values.r = m_r;
		values.g = m_g;
		values.b = m_b;
		values.l = m_l;

		if (global) {
			ALCColorCorrectionValues global = ALCColorCorrectionManager::instance()->correction();
			values.r *= global.r;
			values.g *= global.g;
			values.b *= global.b;
			values.l *= global.l;
		}

		return values;
	}

	void ALCColorCorrection::multiplyCorrectionValues(ALCColorCorrectionValues &values) {
		values.r *= m_r;
		values.g *= m_g;
		values.b *= m_b;
		values.l *= m_l;
	}

	void ALCColorCorrection::clear() {
		m_r = 1.0;
		m_g = 1.0;
		m_b = 1.0;
		m_l = 1.0;
		m_format = Format::RGB;
	}

}
