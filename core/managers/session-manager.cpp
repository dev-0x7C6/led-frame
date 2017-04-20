#include "session-manager.h"

#include <core/correctors/factories/corrector-factory.h>
#include <core/emitters/factories/emitter-factory.h>
#include <core/functionals/debug-notification.h>
#include <core/managers/main-manager.h>
#include <core/receivers/interfaces/ireceiver.h>
#include <core/correctors/interfaces/icorrector.h>

#ifdef GUI
#include <gui/wizards/device-setup-wizard.h>
#endif

#include <QApplication>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>

using namespace Enum;
using namespace Manager;
using namespace Emitter::Factory;
using namespace Emitter::Concrete;
using namespace Corrector::Factory;

SessionManager::SessionManager(QSettings &settings, MainManager &mainManager)
		: m_settings(settings)
		, m_mainManager(mainManager)

{
#ifndef RPI
	auto screens = QGuiApplication::screens();

	for (auto screen : screens) {
		auto emitter = EmitterFactory::create(EmitterType::Screen);
		emitter->setName(QObject::tr("Display: ") + screen->name());
		m_mainManager.emitters().attach(emitter);
	}
#endif

#ifdef RPI
	auto emitter = EmitterFactory::create(EmitterType::Screen);
	emitter->setName("RPI Output");
	m_mainManager.emitters().attach(emitter);
#endif

	const auto list = {
		EmitterType::Image,
		EmitterType::Animation,
		EmitterType::Color,
		EmitterType::Off};

	for (const auto &type : list)
		m_mainManager.emitters().attach(EmitterFactory::create(type, translate(type)));

	m_mainManager.receivers().setRegisterDeviceCallback([this](Receiver::Interface::IReceiver *receiver, const QString &serialNumber) -> bool {
		return registerDevice(receiver, serialNumber);
	});
}

bool SessionManager::registerDevice(Receiver::Interface::IReceiver *receiver, const QString &serialNumber) {
#ifdef QT_DEBUG
	receiver->correctorManager().attach(&Functional::DebugNotification::instance());
#endif
	m_settings.beginGroup("devices");
	m_settings.beginGroup(serialNumber);

	if (m_settings.value("name", "").toString().isEmpty()) {
#ifdef GUI
		QMessageBox::information(nullptr, QObject::tr("Led frame detected"), QObject::tr("Detected new led frame device, application will run setup wizard."), QMessageBox::Ok);
		Wizard::DeviceSetupWizard wizard(receiver);
		wizard.exec();
		receiver->setName(wizard.field("deviceName").toString());
		m_settings.setValue("name", wizard.field("deviceName").toString());
#else
		receiver->setName(serialNumber);
		m_settings.setValue("name", serialNumber);
#endif
	} else
		receiver->setName(m_settings.value("name", "").toString());

	m_settings.endGroup();
	m_settings.endGroup();
	m_settings.sync();

	createCorrectorGroup(receiver);

	m_settings.beginGroup(receiver->name());
	const auto defaultEmitter = m_settings.value("emitter").toString();

	if (!defaultEmitter.isEmpty()) {
		for (const auto &emitter : m_mainManager.emitters().list()) {
			if (emitter->name() == defaultEmitter) {
				receiver->connectEmitter(emitter);
				break;
			}
		}
	}
	m_settings.endGroup();
	return true;
}

void SessionManager::createCorrectorGroup(Receiver::Interface::IReceiver *receiver) {
	m_mainManager.correctors().enumerate([receiver](const auto &corrector) {
		receiver->correctorManager().attach(corrector);
	});

	const auto id = receiver->id();

	const auto list = {
		CorrectorType::Brightness,
		CorrectorType::RedChannel,
		CorrectorType::GreenChannel,
		CorrectorType::BlueChannel,
		CorrectorType::FlickrEffect,
		CorrectorType::ColorEnhancer,
		CorrectorType::Backlight,
	};

	m_settings.beginGroup(receiver->name());
	for (const auto &type : list) {
		auto corrector = CorrectorFactory::create(type, id);
		m_settings.beginGroup(value(corrector->type()));
		corrector->setFactor(m_settings.value("factor", corrector->factor().value()).toUInt());
		corrector->setEnabled(m_settings.value("enabled", corrector->isEnabled()).toBool());
		receiver->correctorManager().attach(corrector);
		m_settings.endGroup();
	};

	m_settings.endGroup();
}

SessionManager::~SessionManager() {
	for (const auto &receiver : m_mainManager.receivers().list()) {
		m_settings.beginGroup(receiver->name());

		m_settings.setValue("emitter", "");
		if (receiver->isEmitterConnected()) {
			const auto name = receiver->connectedEmitter()->name();
			m_settings.setValue("emitter", name);
		}

		m_mainManager.correctors().enumerate([this](const auto &corrector) {
			if (corrector->isGlobal())
				return;

			m_settings.beginGroup(value(corrector->type()));
			m_settings.setValue("factor", corrector->factor().value());
			m_settings.setValue("enabled", corrector->isEnabled());
			m_settings.endGroup();
		});

		m_settings.endGroup();
	}
}
