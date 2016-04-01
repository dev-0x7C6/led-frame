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
namespace Device {
	class DeviceThread;
}
class ALCDeviceManager;

namespace Emitters {
	class ALCEmitter;
}

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
	void setCustomEmitter(Device::DeviceThread *, int);
	void setEmitter(ALCReceiver *, Emitters::ALCEmitter *);
};

#include <QCommandLinkButton>

class DeviceLinkButton : public QCommandLinkButton {
private:
	Device::DeviceThread *m_device;

public:
	void setDeviceThread(Device::DeviceThread *thread) {
		m_device = thread;
	}

	Device::DeviceThread *deviceThread() {
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
	void deviceConnected(Device::DeviceThread *);
	void deviceDisconnected(Device::DeviceThread *);
	void addWorkspace(ALCDeviceTreeWidget *, Device::DeviceThread *);
	void populate(QString use);
	void reconfigure();
	void setEmitter(ALCReceiver *device, Emitters::ALCEmitter *emitter);

	void configureEmitter();

};
