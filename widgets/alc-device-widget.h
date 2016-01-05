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

#ifndef ALCDEVICEWIDGET_H
#define ALCDEVICEWIDGET_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QObject>
#include <QComboBox>

#include "connector/alc-receiver.h"

class ALCSymulationWidget;
class QSettings;

namespace Ui {
	class ALCDeviceWidget;
}

class AmbientDeviceThread;
class ALCDeviceManager;

namespace Emitters {
	class ALCEmitter;
}

class ComboBoxItem : public QComboBox {
	Q_OBJECT
private:
	QTreeWidgetItem *item;
	int column;

public:
	ComboBoxItem(QTreeWidgetItem *, int);

public slots:
	void changeItem(int);
};


class ALCDeviceTreeWidget : public QObject, public QTreeWidgetItem {
	Q_OBJECT
private:
	ALCReceiver *m_receiver;

public:
	ALCDeviceTreeWidget(QTreeWidget *tree, ALCReceiver *receiver)
		: QObject(0),
		  QTreeWidgetItem(tree) {
		m_receiver = receiver;
	}

	ALCReceiver *receiver() {
		return m_receiver;
	}
	void setReceiver(ALCReceiver *receiver) {
		m_receiver = receiver;
	}
	void currentIndexChanged(int);

signals:
	void setCustomEmitter(AmbientDeviceThread *, int);
	void setEmitter(ALCReceiver *, Emitters::ALCEmitter *);
};

#include <QCommandLinkButton>

class DeviceLinkButton : public QCommandLinkButton {
private:
	AmbientDeviceThread *m_device;

public:
	void setDeviceThread(AmbientDeviceThread *thread) {
		m_device = thread;
	}

	AmbientDeviceThread *deviceThread() {
		return m_device;
	}
};

class ALCDeviceWidget : public QMainWindow {
	Q_OBJECT
private:
	Ui::ALCDeviceWidget *ui;
	QList <ALCDeviceTreeWidget *> m_devices;
	ALCDeviceManager *m_manager;
	ALCSymulationWidget *m_symulation;
	QSettings *m_settings;

public:
	explicit ALCDeviceWidget(QWidget *parent = 0);
	~ALCDeviceWidget();

	void addSymulation(ALCSymulationWidget *);

private:
	void deviceConnected(AmbientDeviceThread *);
	void deviceDisconnected(AmbientDeviceThread *);
	void addWorkspace(ALCDeviceTreeWidget *, AmbientDeviceThread *);
	void populate(QString use);
	void reconfigure();
	void setEmitter(ALCReceiver *device, Emitters::ALCEmitter *emitter);

	void configureEmitter();

};

#endif // ALCDEVICEWIDGET_H
