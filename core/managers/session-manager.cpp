#include "session-manager.h"

#include <core/correctors/factories/corrector-factory.h>
#include <core/emitters/factories/emitter-factory.h>
#include <core/functionals/debug-notification.h>
#include <core/managers/main-manager.h>
#include <core/interfaces/ireceiver.h>
#include <core/interfaces/icorrector.h>
#include <core/enums/animation-variant.h>

#include <QApplication>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>

using namespace Enum;
using namespace Manager;
using namespace Emitter::Factory;
using namespace Corrector::Factory;

SessionManager::SessionManager(QSettings &settings, MainManager &mainManager)
		: m_settings(settings)
		, m_mainManager(mainManager)

{
#ifndef RPI
	auto screens = QGuiApplication::screens();

	for (auto screen : screens) {
		auto emitter = EmitterFactory::create(EmitterType::Screen);
		emitter->setName((QObject::tr("Display: ") + screen->name()).toStdString());
		m_mainManager.atoms().attach(emitter);
	}
#endif

#ifdef RPI
	auto emitter = EmitterFactory::create(EmitterType::Screen);
	emitter->setName("RPI Output");
	m_mainManager.emitters().attach(emitter);
#endif

	m_mainManager.atoms().attach(EmitterFactory::create(EmitterType::Image, translate(EmitterType::Image)));

	for (auto &types : getAnimationVariantTypes()) {
		auto animation = EmitterFactory::create(EmitterType::Animation, translate(EmitterType::Animation));
		animation->interpret(types);
		m_mainManager.atoms().attach(std::move(animation));
	}

	m_mainManager.atoms().attach(EmitterFactory::create(EmitterType::Color, translate(EmitterType::Color)));
	m_mainManager.atoms().attach(EmitterFactory::create(EmitterType::Off, translate(EmitterType::Off)));

	m_mainManager.setRegisterDeviceCallback([this](IReceiver *receiver, const QString &serialNumber) -> bool {
		return registerDevice(receiver, serialNumber);
	});
}

bool SessionManager::registerDevice(IReceiver *receiver, const QString &serialNumber) {
#ifdef QT_DEBUG
	receiver->correctors().attach(&Functional::DebugNotification::instance());
#endif
	m_settings.beginGroup("devices");
	m_settings.beginGroup(serialNumber);

	if (m_settings.value("name", "").toString().isEmpty()) {
		receiver->setName(serialNumber.toStdString());
		m_settings.setValue("name", serialNumber);
	} else
		receiver->setName(m_settings.value("name", "").toString().toStdString());

	m_settings.endGroup();
	m_settings.endGroup();
	m_settings.sync();

	createCorrectorGroup(receiver);

	m_settings.beginGroup(QString::fromStdString(receiver->name()));
	const auto defaultEmitter = m_settings.value("emitter").toString().toStdString();

	if (!defaultEmitter.empty()) {

		m_mainManager.atoms().enumerate([receiver, defaultEmitter](const auto &atom) {
			if (Category::Emitter != atom->category())
				return;

			auto emitter = std::static_pointer_cast<IEmitter>(atom);
			if (emitter->name() == defaultEmitter)
				receiver->connectEmitter(emitter);
		});
	}
	m_settings.endGroup();
	return true;
}

void SessionManager::createCorrectorGroup(IReceiver *receiver) {
	m_mainManager.atoms().enumerate([receiver](const auto &atom) {
		if (Category::Corrector == atom->category())
			receiver->correctors().attach(atom);
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

	m_settings.beginGroup(QString::fromStdString(receiver->name()));
	for (const auto &type : list) {
		auto corrector = CorrectorFactory::create(type, id);
		m_settings.beginGroup(value(corrector->type()).c_str());
		corrector->setFactor(m_settings.value("factor", corrector->factor().value()).toUInt());
		corrector->setEnabled(m_settings.value("enabled", corrector->isEnabled()).toBool());
		receiver->correctors().attach(corrector);
		m_settings.endGroup();
	};

	m_settings.endGroup();
}

SessionManager::~SessionManager() {
	m_mainManager.atoms().enumerate([this](const auto &atom) {
		if (Category::Receiver != atom->category())
			return;

		auto receiver = std::static_pointer_cast<IReceiver>(atom);

		m_settings.beginGroup(QString::fromStdString(receiver->name()));

		m_settings.setValue("emitter", "");
		if (receiver->isEmitterConnected()) {
			const auto name = receiver->connectedEmitter()->name();
			m_settings.setValue("emitter", QString::fromStdString(name));
		}

		m_mainManager.atoms().enumerate([this](const auto &atom) {
			if (Category::Corrector != atom->category())
				return;

			auto corrector = std::static_pointer_cast<ICorrector>(atom);
			m_settings.beginGroup(value(corrector->type()).c_str());
			m_settings.setValue("factor", corrector->factor().value());
			m_settings.setValue("enabled", corrector->isEnabled());
			m_settings.endGroup();
		});

		m_settings.endGroup();
	});
}
