#include "session-manager.h"

#include <core/correctors/factories/corrector-factory.h>
#include <core/emitters/concretes/camera-emitter.h>
#include <core/emitters/factories/emitter-factory.h>
#include <core/enums/animation-variant.h>
#include <core/functionals/debug-notification.h>
#include <core/interfaces/icorrector.h>
#include <core/interfaces/iemitter.h>
#include <core/interfaces/ireceiver.h>
#include <core/managers/main-manager.h>
#include <externals/common/qt/raii/raii-settings-group.hpp>

#include <QApplication>
#include <QScreen>
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
		auto emitter = make_emitter(EmitterType::Camera, info.description().toStdString(), info);

		m_unregisterCallable[emitter->id()] = [this, info]() {
			m_cameraManager->unmark(info);
		};

		m_mainManager.atoms().attach(std::move(emitter));
	});

	QObject::connect(&m_invalidateTimer, &QTimer::timeout, &m_invalidateTimer, [this]() {
		std::queue<i32> queue;

		m_mainManager.atoms().enumerate([&queue](const std::shared_ptr<IRepresentable> &value) {
			if (Category::Emitter == value->category()) {
				auto emitter = std::dynamic_pointer_cast<IEmitter>(value);
				if (emitter->isValid())
					return;

				queue.emplace(emitter->id());
			}
		});

		while (!queue.empty()) {
			auto &&emitter = queue.front();
			if (m_unregisterCallable.count(emitter)) {
				m_unregisterCallable[emitter]();
				m_unregisterCallable.erase(emitter);
			}
			m_mainManager.atoms().detach(emitter);
			queue.pop();
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

	for (auto &&type : animationVariantTypes)
		m_mainManager.atoms().attach(make_emitter(EmitterType::Animation, translate(type), type));

	m_mainManager.atoms().attach(make_emitter(EmitterType::Color, translate(EmitterType::Color)));
	m_mainManager.atoms().attach(make_emitter(EmitterType::Off, translate(EmitterType::Off)));

	m_mainManager.setRegisterDeviceCallback([this](IReceiver &receiver) -> bool {
		return registerDevice(receiver);
	});
}

bool SessionManager::registerDevice(IReceiver &receiver) {
#ifdef QT_DEBUG
	receiver.correctors().attach(&Functional::DebugNotification::instance());
#endif
	raii_settings_group _(m_settings, receiver.name());
	createCorrectorGroup(m_settings, receiver);

	const auto last_selected_emitter = m_settings.value("last_selected_emitter").toString().toStdString();

	if (!last_selected_emitter.empty()) {
		m_mainManager.atoms().enumerate([&receiver, last_selected_emitter](const auto &atom) {
			if (Category::Emitter != atom->category())
				return;

			auto emitter = std::static_pointer_cast<IEmitter>(atom);
			if (emitter->name() == last_selected_emitter)
				receiver.connectEmitter(emitter);
		});
	}
	return true;
}

void SessionManager::createCorrectorGroup(QSettings &settings, IReceiver &receiver) {
	m_mainManager.atoms().enumerate([&receiver](const auto &atom) {
		if (Category::Corrector == atom->category())
			receiver.correctors().attach(atom);
	});

	const auto id = receiver.id();

	raii_settings_group _(settings, "correctors");
	for (const auto &type : getCorrectorTypes()) {
		std::shared_ptr corrector = make_corrector(type, id);
		raii_settings_group _(settings, value(type));
		corrector->load(settings);
		receiver.correctors().attach(corrector);
	}
}

SessionManager::~SessionManager() {
	m_mainManager.atoms().enumerate([this](const auto &atom) {
		if (Category::Receiver != atom->category())
			return;

		auto receiver = std::static_pointer_cast<IReceiver>(atom);

		raii_settings_group _(m_settings, receiver->name());

		if (const auto emitter = receiver->connectedEmitter(); emitter)
			m_settings.setValue("last_selected_emitter", QString::fromStdString(emitter->name()));

		raii_settings_group __(m_settings, "correctors");
		receiver->correctors().enumerate([this](const std::shared_ptr<IRepresentable> &v) {
			auto &&corrector = std::static_pointer_cast<ICorrector>(v);
			raii_settings_group _(m_settings, value(corrector->type()));
			corrector->save(m_settings);
		});
	});
}
