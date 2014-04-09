#include "color-emitter.h"

#include <QTreeWidgetItem>

ColorEmitter::ColorEmitter()
  :m_type(EMITTER_NOT_DEFINED),
   m_treeItem(0)
{
  m_connectedCount = 0;
}

ColorEmitter::~ColorEmitter() {
  //  delete m_treeItem;
}

void ColorEmitter::setEmitterName(const QString &name) {
  m_emitterName = name;
}

QString ColorEmitter::emitterName() const {
  return m_emitterName;
}

ColorEmitter::EmitterType ColorEmitter::type() const { return m_type; }

void ColorEmitter::init() {
  QMutexLocker locker(&m_mutex);
  m_connectedCount++;
 // qDebug() << this << " device connected, count: " << m_connectedCount;
}

void ColorEmitter::done() {
  QMutexLocker locker(&m_mutex);
  m_connectedCount--;
//  qDebug() << this << " device disconnected, count: " << m_connectedCount;
}

void ColorEmitter::setState(ALCColorSamples &samples) {
  QMutexLocker locker(&m_mutex);
  m_samples.copy(samples);
}

void ColorEmitter::state(ALCColorSamples &samples) {
  QMutexLocker locker(&m_mutex);
  samples.copy(m_samples);
}

void ColorEmitter::setTreeItem(QTreeWidgetItem *item) {
  m_treeItem = item;
}

QTreeWidgetItem *ColorEmitter::treeItem() {
  return m_treeItem;
}

bool ColorEmitter::configure() {
  return false;
}

#include <QInputDialog>
#include <QApplication>

bool ColorEmitter::rename() {
  QString text = QInputDialog::getText(0, "Rename", "Set name:", QLineEdit::Normal, emitterName());
  if (!text.isEmpty()) {
    setEmitterName(text);
  }

  return text.isEmpty();
}

#include <QMessageBox>

#include "managers/alc-emitter-manager.h"

bool ColorEmitter::remove() {
  if (QMessageBox::question(0, "Question", "Do you realy want to delete this emitter.",
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    return false;

  ALCEmitterManager::instance()->remove(this);
}

