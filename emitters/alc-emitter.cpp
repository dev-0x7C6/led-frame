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

#include "alc-emitter.h"

#include <QTreeWidgetItem>

ALCEmitter::ALCEmitter()
  :m_treeItem(0),
   m_type(EMITTER_NOT_DEFINED)
{
  m_connectedCount = 0;
}

ALCEmitter::~ALCEmitter() {
  //  delete m_treeItem;
}

void ALCEmitter::setEmitterName(const QString &name) {
  m_emitterName = name;
}

QString ALCEmitter::emitterName() const {
  return m_emitterName;
}

ALCEmitter::EmitterType ALCEmitter::type() const { return m_type; }

void ALCEmitter::init() {
  QMutexLocker locker(&m_mutex);
  m_connectedCount++;
 // qDebug() << this << " device connected, count: " << m_connectedCount;
}

void ALCEmitter::done() {
  QMutexLocker locker(&m_mutex);
  m_connectedCount--;
//  qDebug() << this << " device disconnected, count: " << m_connectedCount;
}

void ALCEmitter::setState(ALCColorSamples &samples) {
  QMutexLocker locker(&m_mutex);
  m_safeSamples.copy(samples);
}

void ALCEmitter::state(ALCColorSamples &samples) {
  QMutexLocker locker(&m_mutex);
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

#include <QInputDialog>
#include <QApplication>

bool ALCEmitter::rename() {
  QString text = QInputDialog::getText(0, "Rename", "Set name:", QLineEdit::Normal, emitterName());
  if (!text.isEmpty()) {
    setEmitterName(text);
  }

  return text.isEmpty();
}

#include <QMessageBox>

#include "managers/alc-emitter-manager.h"

bool ALCEmitter::remove() {
  if (QMessageBox::question(0, "Question", "Do you realy want to delete this emitter.",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    return false;

  ALCEmitterManager::instance()->remove(this);
  return true;
}

