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

#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "emitters/alc-emitter.h"
#include "managers/alc-emitter-manager.h"

namespace Emitters {

	ALCEmitter::ALCEmitter(ALCEmitter::Type type)
		: m_treeItem(0),
		  m_type(type) {
		m_connectedCount = 0;
	}

	ALCEmitter::~ALCEmitter() {
	}

	void ALCEmitter::setEmitterName(const QString &name) {
		m_emitterName = name;
	}

	QString ALCEmitter::emitterName() const {
		return m_emitterName;
	}

	void ALCEmitter::setType(const ALCEmitter::Type type) {
		m_type = type;
	}

	ALCEmitter::Type ALCEmitter::type() const {
		return m_type;
	}

	void ALCEmitter::init() {
		m_connectedCount++;
		qDebug() << this << " device connected, count: " << m_connectedCount;
	}

	void ALCEmitter::done() {
		m_connectedCount--;
		qDebug() << this << " device disconnected, count: " << m_connectedCount;
	}

	void ALCEmitter::setState(const ALCColorSamples &samples) {
		QMutexLocker locaker(&m_mutex);
		m_safeSamples.copy(samples);
	}

	void ALCEmitter::state(ALCColorSamples &samples) {
		QMutexLocker locaker(&m_mutex);
		samples.copy(m_safeSamples);
	}

	void ALCEmitter::setTreeItem(QTreeWidgetItem *item) {
		m_treeItem = item;
	}

	QTreeWidgetItem *ALCEmitter::treeItem() {
		return m_treeItem;
	}

	bool ALCEmitter::configure() {
		return false;
	}

	bool ALCEmitter::rename() {
		QString text = QInputDialog::getText(0, "Rename", "Set name:", QLineEdit::Normal, emitterName());

		if (!text.isEmpty())
			setEmitterName(text);

		return text.isEmpty();
	}

	bool ALCEmitter::remove() {
		if (QMessageBox::question(0, "Question", "Do you realy want to delete this emitter.",
		                          QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
			return false;

		ALCEmitterManager::instance()->remove(this);
		return true;
	}

}
