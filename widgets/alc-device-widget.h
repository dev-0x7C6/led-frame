#pragma once

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
