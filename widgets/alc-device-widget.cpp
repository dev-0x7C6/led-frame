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

#include "alc-device-widget.h"
#include "ui_alc-device-widget.h"

#include "classes/alc-settings.h"
#include "managers/alc-device-manager.h"
#include "connector/alc-device-thread.h"
#include "emitters/alc-emitter.h"
#include "emitters/alc-emitter.h"
#include "managers/alc-emitter-manager.h"
#include "widgets/alc-symulation-widget.h"

#include <QCommandLinkButton>
#include <QMessageBox>
#include <QSettings>

ALCDeviceWidget::ALCDeviceWidget(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ALCDeviceWidget),
	m_settings(ALCSettings::instance()->settings()) {
	ui->setupUi(this);
	//connect(m_manager, &ALCDeviceManager::deviceConnected, this, &ALCDeviceWidget::deviceConnected, Qt::DirectConnection);
	//connect(m_manager, &ALCDeviceManager::deviceDisconnected, this, &ALCDeviceWidget::deviceDisconnected, Qt::DirectConnection);
	connect(ALCEmitterManager::instance(), &ALCEmitterManager::emitterListChanged, this, &ALCDeviceWidget::reconfigure);
}


ALCDeviceWidget::~ALCDeviceWidget() {
	m_settings->beginGroup("defaults");
	ALCReceiver *receiver;
	Emitters::ALCEmitter *emitter;

	for (int i = 0; i < m_devices.count(); ++i) {
		receiver = m_devices[i]->receiver();

		if (!receiver)
			continue;

		emitter = receiver->connectedEmitter();

		if (emitter)
			m_settings->setValue(receiver->name(), emitter->emitterName());
		else
			m_settings->setValue(receiver->name(), QString());
	}

	m_settings->endGroup();
	delete ui;
}

ComboBoxItem::ComboBoxItem(QTreeWidgetItem *item, int column) {
	this->item = item;
	this->column = column;
	connect(this, SIGNAL(currentIndexChanged(int)), SLOT(changeItem(int)));
}

void ComboBoxItem::changeItem(int index) {
	if (index >= 0)
		item->setData(this->column, Qt::UserRole, itemText(index));
}

void ALCDeviceTreeWidget::currentIndexChanged(int idx) {
	Q_UNUSED(idx)
	ComboBoxItem *cmb = dynamic_cast <ComboBoxItem *>(sender());
	emit setEmitter(m_receiver, reinterpret_cast<Emitters::ALCEmitter *>(qvariant_cast <void *>(cmb->currentData())));
}

void ALCDeviceWidget::addSymulation(ALCSymulationWidget *symulation) {
	m_symulation = symulation;
	ALCDeviceTreeWidget *item = new ALCDeviceTreeWidget(ui->tree, 0);
	item->setReceiver(m_symulation);
	connect(item, &ALCDeviceTreeWidget::setEmitter, this, &ALCDeviceWidget::setEmitter);
	item->setText(0, "Symulation");
	item->setIcon(0, QIcon(":/22x22/leds.png"));
	addWorkspace(item, 0);
	m_devices << item;
	ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
	populate(m_settings->value("symulation").toString());
}

void ALCDeviceWidget::deviceConnected(AmbientDeviceThread *) {
	//  ALCDeviceTreeWidget *item = new ALCDeviceTreeWidget(ui->tree, thread);
	//  connect(item, &ALCDeviceTreeWidget::setEmitter, this, &ALCDeviceWidget::setEmitter);
	//  item->setText(0, thread->details().systemLocation() + '\t');
	//  item->setIcon(0, QIcon(":/22x22/device.png"));
	//  addWorkspace(item, thread);
	//  ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
	//  m_devices << item;
	//  populate(m_settings->value(thread->details().systemLocation()).toString());
}

void ALCDeviceWidget::deviceDisconnected(AmbientDeviceThread *thread) {
	for (int i = 0; i < m_devices.count(); ++i) {
		if (thread == dynamic_cast<AmbientDeviceThread *>(m_devices[i]->receiver())) {
			delete m_devices[i];
			m_devices.removeAt(i);
			return;
		}
	}
}

void ALCDeviceWidget::addWorkspace(ALCDeviceTreeWidget *item, AmbientDeviceThread *thread) {
	QTreeWidgetItem *child = new QTreeWidgetItem(item);
	QPalette p = palette();
	p.setColor(QPalette::Background, QColor(226, 237, 253));
	QWidget *color = new QWidget(0);
	color->setPalette(p);
	color->setBackgroundRole(QPalette::Background);
	color->setAutoFillBackground(true);
	QWidget *workspace = new QWidget(0);
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	workspace->setPalette(p);
	workspace->setBackgroundRole(QPalette::Background);
	workspace->setLayout(layout);
	workspace->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	workspace->setAutoFillBackground(true);
	DeviceLinkButton *linkLedStrip = new DeviceLinkButton;
	DeviceLinkButton *linkEmitter = new DeviceLinkButton;
	DeviceLinkButton *linkDevice = new DeviceLinkButton;
	linkDevice->setDeviceThread(thread);
	linkDevice->setText("Device settings");
	linkDevice->setIconSize(QSize(22, 22));
	linkDevice->setIcon(QIcon(":/22x22/device.png"));
	linkDevice->setDescription("You can set there setings like: device brightness, update speed");
	linkDevice->setMaximumHeight(50);
	linkEmitter->setDeviceThread(thread);
	linkEmitter->setIconSize(QSize(22, 22));
	linkEmitter->setIcon(QIcon(":/22x22/leds.png"));
	linkEmitter->setText("Emitter settings");
	linkEmitter->setDescription("Quick settings for your currently selected emitter");
	linkEmitter->setMaximumHeight(50);
	connect(linkEmitter, &DeviceLinkButton::clicked, this, &ALCDeviceWidget::configureEmitter);
	linkLedStrip->setDeviceThread(thread);
	linkLedStrip->setIconSize(QSize(22, 22));
	linkLedStrip->setIcon(QIcon(":/22x22/chunks.png"));
	linkLedStrip->setText("Strip configuration");
	linkLedStrip->setDescription("Wizard configurator for led strips and your monitor");
	linkLedStrip->setMaximumHeight(50);
	linkDevice->setEnabled(false);
	linkLedStrip->setEnabled(false);
	layout->addWidget(linkDevice);
	layout->addWidget(linkEmitter);
	layout->addWidget(linkLedStrip);
	ui->tree->setItemWidget(child, 1, workspace);
	ui->tree->setItemWidget(child, 0, color);
}

void ALCDeviceWidget::populate(QString use) {
	const QString prefix = "Emitter: ";

	for (int i = 0; i < m_devices.count(); ++i) {
		ui->tree->removeItemWidget(m_devices[i], 1);
		ComboBoxItem *cmb = new ComboBoxItem(m_devices[i], 1);
		cmb->setIconSize(QSize(22, 22));
		cmb->addItem(QIcon(":/22x22/no-device.png"), QString("Not assigned"));
		QList <Emitters::ALCEmitter *> emitters = ALCEmitterManager::instance()->allEmitters();
		QListIterator <Emitters::ALCEmitter *> ii(emitters);
		Emitters::ALCEmitter *emitter;
		Emitters::ALCEmitter *defaultEmitter = 0;

		while (ii.hasNext()) {
			switch ((emitter = ii.next())->type()) {
				case Emitters::ALCEmitter::Type::ScreenCapture:
					cmb->addItem(QIcon(":/22x22/screen.png"), prefix + emitter->emitterName(),
					             qVariantFromValue((void *)emitter));
					break;

				case Emitters::ALCEmitter::Type::PlainColor:
					cmb->addItem(QIcon(":/22x22/color.png"),  prefix + emitter->emitterName(),
					             qVariantFromValue((void *)emitter));
					break;

				case Emitters::ALCEmitter::Type::Animation:
					cmb->addItem(QIcon(":/22x22/animation.png"), prefix + emitter->emitterName(),
					             qVariantFromValue((void *)emitter));
					break;

				case Emitters::ALCEmitter::Type::Image:
					cmb->addItem(QIcon(":/22x22/from-image.png"), prefix + emitter->emitterName(),
					             qVariantFromValue((void *)emitter));
					break;

				default:
					break;
			}

			Emitters::ALCEmitter *connected = m_devices[i]->receiver()->connectedEmitter();
			m_settings->beginGroup("defaults");
			use = m_settings->value(m_devices[i]->receiver()->name()).toString();
			m_settings->endGroup();

			if (connected && (connected == emitter))
				cmb->setCurrentIndex(cmb->count() - 1);

			if (!use.isEmpty() && (emitter->emitterName() == use)) {
				defaultEmitter = emitter;
				cmb->setCurrentIndex(cmb->count() - 1);
			}
		}

		m_devices[i]->receiver()->connectEmitter(defaultEmitter);
		connect(cmb, static_cast <void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		        m_devices[i], &ALCDeviceTreeWidget::currentIndexChanged, Qt::DirectConnection);
		ui->tree->setItemWidget(m_devices[i], 1, cmb);
	}
}

void ALCDeviceWidget::reconfigure() {
	populate("");
}

void ALCDeviceWidget::setEmitter(ALCReceiver *receiver, Emitters::ALCEmitter *emitter) {
	receiver->connectEmitter(emitter);
}


void ALCDeviceWidget::configureEmitter() {
	//  DeviceLinkButton *link = dynamic_cast <DeviceLinkButton *>(sender());
	//  Emitters::ALCEmitter *emitter;
	//  if (link->deviceThread())
	//    emitter = link->deviceThread()->connectedEmitter();
	//  else
	//    emitter = m_symulation->connectedEmitter();
	//  if (emitter)
	//    emitter->configure();
	//  else
	//    QMessageBox::information(this, "Information", "Emitter is not defined.", QMessageBox::Ok);
}
