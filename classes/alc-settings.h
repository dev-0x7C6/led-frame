#pragma once

class QSettings;

class ALCSettings {
private:
	QSettings *m_settings;
public:
	explicit ALCSettings();
	virtual ~ALCSettings();

	QSettings *settings();

	void load();
	void save();

	static ALCSettings *instance() {
		static ALCSettings settings;
		return &settings;
	}

};
