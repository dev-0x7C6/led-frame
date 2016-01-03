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

#ifndef ALCEMITTER_H
#define ALCEMITTER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QDebug>

#include <atomic>

#include "classes/alc-color-samples.h"
#include "correctors/alc-color-correction.h"

class QTreeWidgetItem;

namespace Emitters {

	class ALCEmitter : public Correctors::ALCColorCorrection {
	public:
		enum class Type {
			None,
			ScreenCapture,
			Blackhole,
			Animation,
			PlainColor,
			Image,
			Last
		};

	private:
		QString m_emitterName;
		ALCColorSamples m_safeSamples;
		QTreeWidgetItem *m_treeItem;

	protected:
		Type m_type;
		QMutex m_mutex;

		std::atomic <int> m_connectedCount;

	public:
		explicit ALCEmitter(ALCEmitter::Type type);
		virtual ~ALCEmitter();

		void setEmitterName(const QString &name);
		QString emitterName() const;

		ALCEmitter::Type type() const;
		void setType(const ALCEmitter::Type type);

		virtual void init();
		virtual void done();

		void setState(const ALCColorSamples &samples);
		void state(ALCColorSamples &samples);

		void setTreeItem(QTreeWidgetItem *item);
		QTreeWidgetItem *treeItem();

		virtual bool configure();
		virtual bool rename();
		virtual bool remove();
	};

}

#endif // ALCEMITTER_H
