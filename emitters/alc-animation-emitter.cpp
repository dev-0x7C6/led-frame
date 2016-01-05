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

#include <QMessageBox>
#include <QVariantAnimation>
#include <QDebug>

#include "emitters/alc-animation-emitter.h"
#include "dialogs/alc-animation-configure-dialog.h"

namespace Emitters {

	ALCAnimationEmitter::ALCAnimationEmitter() :
		QThread(),
		ALCEmitter(ALCEmitter::Type::Animation),
		m_animation(Animation::Glow),
		m_effect(Effect::Flicker),
		m_rgb(0),
		m_flicker(1),
		m_quit(false),
		m_variantAnimation(this) {
		connect(&m_variantAnimation, &QVariantAnimation::valueChanged, this, &ALCAnimationEmitter::process);
		setup(m_animation, m_effect, 10000);
	}

	ALCAnimationEmitter::~ALCAnimationEmitter() {
		m_quit = true;
		wait();
	}

	bool ALCAnimationEmitter::open() {
		QMessageBox::warning(0, "Warning", "To implement.", QMessageBox::Ok);
		return false;
	}

	bool ALCAnimationEmitter::configure() {
		ALCAnimationConfigureDialog dialog;
		dialog.exec();
		return false;
	}

	void ALCAnimationEmitter::setup(Animation animation, Effect effect, int cycle) {
		if (m_variantAnimation.state() == QVariantAnimation::Running)
			m_variantAnimation.stop();

		m_variantAnimation.setKeyValues(QVariantAnimation::KeyValues());
		m_variantAnimation.setDuration(cycle);

		switch (m_animation = animation) {
			case Animation::None:
				break;

			case Animation::Shift:
				m_variantAnimation.setKeyValueAt(0.000, QColor::fromRgbF(1, 0, 0));
				m_variantAnimation.setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
				m_variantAnimation.setKeyValueAt(0.666, QColor::fromRgbF(0, 0, 1));
				m_variantAnimation.setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
				break;

			case Animation::Glow:
				m_variantAnimation.setKeyValueAt(0.000, QColor::fromRgbF(1, 0, 0));
				m_variantAnimation.setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
				m_variantAnimation.setKeyValueAt(0.666, QColor::fromRgbF(0, 0, 1));
				m_variantAnimation.setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
				break;

			case Animation::Rotate:
				m_variantAnimation.setKeyValueAt(0.000, QColor::fromRgbF(1, 0, 0));
				m_variantAnimation.setKeyValueAt(0.333, QColor::fromRgbF(0, 1, 0));
				m_variantAnimation.setKeyValueAt(0.666, QColor::fromRgbF(0, 0, 1));
				m_variantAnimation.setKeyValueAt(1.000, QColor::fromRgbF(1, 0, 0));
				break;
		}

		switch (m_effect = effect) {
			case Effect::None:
				break;

			case Effect::Flicker:
				break;
		}

		m_variantAnimation.setLoopCount(-1);
	}

	void ALCAnimationEmitter::setFlickerValue(int flicker) {
		m_flicker = flicker;
	}

	int ALCAnimationEmitter::flickerValue() {
		return m_flicker;
	}

	void ALCAnimationEmitter::setQuitState(bool state) {
		m_quit = state;
	}

	void ALCAnimationEmitter::run() {
		ALCColorSamples samples;
		Functional::LoopSync sync;
		quint64 counter = 0;
		quint64 loop = 0;
		QVector <QRgb> data;
		data.resize(ALCColorSamples::Resolution * 4);
		quint32 ptr = 0;

		while (!m_quit) {
			counter++;
			int rgb = m_rgb;
			double l = correction(Correctors::ALCColorCorrection::Color::Brightness);
			double r = qMin((qRed(rgb) / 255.0) * correction(Correctors::ALCColorCorrection::Color::Red) * l, 1.0);
			double g = qMin((qGreen(rgb) / 255.0) * correction(Correctors::ALCColorCorrection::Color::Green) * l, 1.0);
			double b = qMin((qBlue(rgb) / 255.0) * correction(Correctors::ALCColorCorrection::Color::Blue) * l, 1.0);
			rgb = QColor::fromRgbF(r, g, b).rgb();
			Effect effect = m_effect;

			switch (effect)  {
				case Effect::None:
					break;

				case Effect::Flicker:
					if (!m_flicker)
						break;

					if (counter % m_flicker < (m_flicker >> 1))
						rgb = 0;

					break;
			}

			Animation animation = m_animation;

			switch (animation) {
				case Animation::None:
					break;

				case Animation::Rotate:
					data[ptr-- % data.count()] = rgb;
					break;

				case Animation::Shift:
					for (int i = 1; i < data.count(); ++i)
						data[i - 1] = data[i];

					data[data.count() - 1] = rgb;
					break;

				case Animation::Glow:
					data.fill(rgb);
					break;
			}

			for (int i = 0; i < 4; ++i)
				memcpy(samples.get(static_cast<ALCColorSamples::Position>(i))->data(),
				       data.data() + (data.size() / 4  * i), data.size());

			setState(samples);

			if (loop != sync.wait(100))
				loop = sync.loopCount();
		}
	}

	void ALCAnimationEmitter::init() {
		ALCEmitter::init();
		m_variantAnimation.start();
		m_quit = false;
		start();
	}

	void ALCAnimationEmitter::done() {
		ALCEmitter::done();

		if (m_variantAnimation.state() == QVariantAnimation::Running)
			m_variantAnimation.stop();

		m_quit = true;
		wait();
	}

	void ALCAnimationEmitter::process(const QVariant &value) {
		m_rgb = (qvariant_cast<QColor>(value)).rgb();
	}

}
