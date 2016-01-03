#include "alc-color-correction-manager.h"
#include "correctors/alc-audio-color-correction.h"
#include "correctors/alc-color-correction.h"
#include "correctors/alc-color-correction-values.h"
#include "correctors/alc-global-color-correction.h"
#include "correctors/alc-weather-color-correction.h"

#include <QSettings>

namespace Correctors {

	const QList <ALCColorCorrection *> ALCColorCorrectionManager::list = {
		ALCAudioColorCorrection::instance(),
		ALCGlobalColorCorrection::instance(),
		ALCWeatherColorCorrection::instance()
	};

	ALCColorCorrectionManager::ALCColorCorrectionManager() {
	}

	ALCColorCorrectionManager::~ALCColorCorrectionManager() {
	}

	ALCColorCorrectionManager *ALCColorCorrectionManager::instance() {
		static ALCColorCorrectionManager manager;
		return &manager;
	}

	ALCColorCorrectionValues ALCColorCorrectionManager::correction() {
		struct ALCColorCorrectionValues values;
		values.r = 1.0;
		values.g = 1.0;
		values.b = 1.0;
		values.l = 1.0;

		for (ALCColorCorrection *corrector : list)
			corrector->multiplyCorrectionValues(values);

		return values;
	}

	double ALCColorCorrectionManager::correction(ALCColorCorrection::Color color) {
		double correction = 1.0;

		for (ALCColorCorrection *corrector : list)
			correction *= corrector->correction(color);

		return correction;
	}

	void ALCColorCorrectionManager::init() {
		load(ALCGlobalColorCorrection::instance(), "Global");
	}

	void ALCColorCorrectionManager::done() {
		save(ALCGlobalColorCorrection::instance(), "Global");
	}

	void ALCColorCorrectionManager::save(ALCColorCorrection *correction, QString name) {
		QSettings settings("AmbientLedDriver", "AmbientLedDriver");
		settings.beginGroup("ColorCorrection");
		settings.beginGroup(name);
		correction->saveCorrection(&settings);
		settings.endGroup();
		settings.endGroup();
	}

	void ALCColorCorrectionManager::load(ALCColorCorrection *correction, QString name) {
		QSettings settings("AmbientLedDriver", "AmbientLedDriver");
		settings.beginGroup("ColorCorrection");
		settings.beginGroup(name);
		correction->loadCorrection(&settings);
		settings.endGroup();
		settings.endGroup();
	}

}
