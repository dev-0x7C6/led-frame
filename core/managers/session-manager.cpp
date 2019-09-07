#include "session-manager.h"

#include <core/correctors/factories/corrector-factory.h>
#include <core/emitters/factories/emitter-factory.h>
#include <core/enums/animation-variant.h>
#include <core/functionals/debug-notification.h>
#include <core/interfaces/icorrector.h>
#include <core/interfaces/iemitter.h>
#include <core/interfaces/ireceiver.h>
#include <core/managers/main-manager.h>
#include <core/emitters/concretes/camera-emitter.h>

#include <QApplication>
#include <QMessageBox>
#include <QScreen>
#include <QSettings>
#include <QCameraInfo>

#include <chrono>

using namespace std::chrono_literals;
using namespace Enum;
using namespace Factory;
using namespace Manager;

class CameraScanner {
public:
	auto scan() {
		auto ret = QCameraInfo::availableCameras();

		ret.erase(std::remove_if(ret.begin(), ret.end(), [this](auto &&x) {
			return m_marked.count(x) == 1;
		}),
			ret.end());

		m_marked.append(ret);
		return ret;
	}

	void unmark(const QCameraInfo &info) {
		m_marked.removeAll(info);
	}

private:
	QList<QCameraInfo> m_marked;
};

class MultiCameraController {
public:
	MultiCameraController(std::function<void(const QCameraInfo &)> &&register_callback) {
		m_scannerInterval.connect(&m_scannerInterval, &QTimer::timeout, [this, callback{std::move(register_callback)}]() {
			for (auto &&camera : m_scanner.scan()) {
				callback(camera);
			}
		});
		m_scannerInterval.setInterval(3s);
		m_scannerInterval.start();
	}

	void unmark(const QCameraInfo &info) {
		m_scanner.unmark(info);
	}

private:
	CameraScanner m_scanner;
	QTimer m_scannerInterval;
};

SessionManager::SessionManager(QSettings &settings, MainManager &mainManager)
		: m_settings(settings)
		, m_mainManager(mainManager)

{
	m_cameraManager = std::make_unique<MultiCameraController>([this](const QCameraInfo &info) {
		m_mainManager.atoms().attach(make_emitter(EmitterType::Camera, info.description().toStdString(), info));
	});

	QObject::connect(&m_invalidateTimer, &QTimer::timeout, &m_invalidateTimer, [this]() {
		std::queue<i32> queue;
		std::queue<QCameraInfo> queue_info;

		m_mainManager.atoms().enumerate([&queue, &queue_info](const std::shared_ptr<IRepresentable> &value) {
			if (Category::Emitter == value->category()) {
				auto emitter = std::static_pointer_cast<IEmitter>(value);
				if (emitter->isValid())
					return;

				queue.emplace(emitter->id());

				//TODO: better way to unregister camera
				auto camera = std::dynamic_pointer_cast<Emitter::Concrete::CameraEmitter>(value);
				if (camera)
					queue_info.emplace(camera->info());
			}
		});

		while (!queue.empty()) {
			m_mainManager.atoms().detach(queue.front());
			queue.pop();
		}

		while (!queue_info.empty()) {
			m_cameraManager->unmark(queue_info.front());
			queue_info.pop();
		}
	});
	m_invalidateTimer.start(100ms);

#ifndef RPI
	auto screens = QGuiApplication::screens();

	for (auto screen : screens) {
		std::shared_ptr emitter = make_emitter(EmitterType::Screen);
		emitter->setName((QObject::tr("Display: ") + screen->name()).toStdString());
		m_mainManager.atoms().attach(emitter);
	}
#endif

#ifdef RPI
	auto emitter = make_emitter(EmitterType::Screen);
	emitter->setName("RPI Output");
	m_mainManager.atoms().attach(std::move(emitter));
#endif

	m_mainManager.atoms().attach(make_emitter(EmitterType::Image, translate(EmitterType::Image)));

	for (auto &&type : animationVariantTypes) {
		auto animation = make_emitter(EmitterType::Animation, translate(EmitterType::Animation));
		animation->interpret(type);
		m_mainManager.atoms().attach(std::move(animation));
	}

	m_mainManager.atoms().attach(make_emitter(EmitterType::Color, translate(EmitterType::Color)));
	m_mainManager.atoms().attach(make_emitter(EmitterType::Off, translate(EmitterType::Off)));

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

	m_settings.beginGroup(QString::fromStdString(receiver->name()));
	for (const auto &type : getCorrectorTypes()) {
		std::shared_ptr corrector = make_corrector(type, id);
		m_settings.beginGroup(value(corrector->type()).c_str());
		corrector->setFactor(m_settings.value("factor", corrector->factor().value()).toUInt());
		corrector->setEnabled(m_settings.value("enabled", corrector->isEnabled()).toBool());
		receiver->correctors().attach(corrector);
		m_settings.endGroup();
	}

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
