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

#include "alc-emitter-widget.h"
#include "ui_alc-emitter-widget.h"

#include <QInputDialog>
#include <QPainter>
#include <QPushButton>

#include "classes/alc-color-samples.h"
#include "emitters/alc-animation-emitter.h"
#include "emitters/alc-emitter.h"
#include "emitters/alc-image-emitter.h"
#include "emitters/alc-color-emitter.h"
#include "emitters/alc-screen-emitter.h"
#include "managers/alc-emitter-manager.h"

#include <QPalette>

ALCEmitterWidget::ALCEmitterWidget(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ALCEmitterWidget) {
	QPalette p = palette();
	p.setColor(QPalette::AlternateBase, QColor(226, 237, 253));
	setPalette(p);
	ui->setupUi(this);
	setup();
	ui->tree->setColumnCount(3);
	ui->tree->header()->setStretchLastSection(false);
	ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
	ui->tree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
}

ALCEmitterWidget::~ALCEmitterWidget() {
	delete ui;
}

void ALCEmitterWidget::addPlainColorItem() {
	QString input = QInputDialog::getText(this, "Name", "Get name:");

	if (!input.isEmpty())
		ALCEmitterManager::instance()->addALCColorEmitter(input);
}

void ALCEmitterWidget::addAnimationItem() {
	QString input = QInputDialog::getText(this, "Name", "Get name:");

	if (!input.isEmpty())
		ALCEmitterManager::instance()->addALCAnimationEmitter(input);
}

void ALCEmitterWidget::addImageItem() {
	QString input = QInputDialog::getText(this, "Name", "Get name:");

	if (!input.isEmpty())
		ALCEmitterManager::instance()->addALCImageEmitter(input);
}

void ALCEmitterWidget::prepare() {
	setup();
}


void ALCEmitterWidget::setup() {
	ui->tree->clear();
	QListIterator <Emitters::ALCEmitter *> ii(ALCEmitterManager::instance()->allEmitters());
	Emitters::ALCEmitter *emitter;

	while (ii.hasNext()) {
		switch ((emitter = ii.next())->type()) {
			case Emitters::ALCEmitter::Type::PlainColor:
				insertPlainColorItem(emitter);
				break;

			case Emitters::ALCEmitter::Type::Animation:
				insertAnimationItem(emitter);
				break;

			case Emitters::ALCEmitter::Type::Image:
				insertImageItem(emitter);
				break;

			default:
				break;
		}
	}
}

void ALCEmitterWidget::insertPlainColorItem(Emitters::ALCEmitter *ptr) {
	Emitters::ALCColorEmitter *emitter = dynamic_cast <Emitters::ALCColorEmitter *>(ptr);
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
	item->setText(0, emitter->emitterName());
	emitter->setTreeItem(item);
	item->setIcon(0, QIcon(":/22x22/color.png"));
	QWidget *workspace = new QWidget();
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	workspace->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	workspace->setLayout(layout);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	insertDefaultButtons(emitter, layout);
	ui->tree->setItemWidget(item, 2, workspace);
	prepareColorItem(item, emitter->color());
}

void ALCEmitterWidget::insertAnimationItem(Emitters::ALCEmitter *ptr) {
	Emitters::ALCAnimationEmitter *emitter = dynamic_cast <Emitters::ALCAnimationEmitter *>(ptr);
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
	item->setText(0, emitter->emitterName());
	emitter->setTreeItem(item);
	item->setIcon(0, QIcon(":/22x22/animation.png"));
	QWidget *workspace = new QWidget();
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	workspace->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	workspace->setLayout(layout);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	insertDefaultButtons(emitter, layout);
	ui->tree->setItemWidget(item, 2, workspace);
}

void ALCEmitterWidget::insertImageItem(Emitters::ALCEmitter *ptr) {
	Emitters::ALCImageEmitter *emitter = dynamic_cast <Emitters::ALCImageEmitter *>(ptr);
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
	item->setText(0, emitter->emitterName());
	item->setText(1, emitter->file());
	item->setTextColor(1, Qt::darkGray);
	emitter->setTreeItem(item);
	item->setIcon(0, QIcon(":/22x22/from-image.png"));
	QWidget *workspace = new QWidget();
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
	workspace->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	workspace->setLayout(layout);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	insertDefaultButtons(emitter, layout);
	ui->tree->setItemWidget(item, 2, workspace);
}

void ALCEmitterWidget::insertDefaultButtons(Emitters::ALCEmitter *emitter, QBoxLayout *layout) {
	QPushButtonEx *configure = new QPushButtonEx;
	QPushButtonEx *rename = new QPushButtonEx;
	QPushButtonEx *remove = new QPushButtonEx;
	configure->setIcon(QIcon(":/16x16/configure.png"));
	configure->setFlat(true);
	configure->setEmitter(emitter);
	rename->setIcon(QIcon(":/16x16/rename.png"));
	rename->setFlat(true);
	rename->setEmitter(emitter);
	remove->setIcon(QIcon(":/16x16/edit-delete.png"));
	remove->setFlat(true);
	remove->setEmitter(emitter);
	connect(configure, &QPushButtonEx::clicked, this, &ALCEmitterWidget::reconfigure);
	connect(remove, &QPushButtonEx::clicked, this, &ALCEmitterWidget::remove);
	connect(rename, &QPushButtonEx::clicked, this, &ALCEmitterWidget::rename);
	layout->addWidget(configure);
	layout->addWidget(rename);
	layout->addWidget(remove);
}

void ALCEmitterWidget::reconfigure() {
	reinterpret_cast <QPushButtonEx *>(sender())->emitter()->configure();
	setup();
}

void ALCEmitterWidget::rename() {
	reinterpret_cast <QPushButtonEx *>(sender())->emitter()->rename();
	setup();
}

void ALCEmitterWidget::remove() {
	reinterpret_cast <QPushButtonEx *>(sender())->emitter()->remove();
	setup();
}

void ALCEmitterWidget::prepareColorItem(QTreeWidgetItem *item, QColor color) {
	item->setTextColor(1, Qt::darkGray);
	item->setText(1, QString("  RGB (%1; %2; %3)").arg(
	                QString::number(color.red()),
	                QString::number(color.green()),
	                QString::number(color.blue())));
	QPixmap pixmap(QSize(16, 16));
	QPainter painter(&pixmap);
	painter.setBrush(color);
	painter.setPen(Qt::darkGray);
	painter.drawRect(0, 0, 15, 15);
	item->setIcon(1, QIcon(pixmap));
}


