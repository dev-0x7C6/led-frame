#pragma once

#include <QSettings>

class settings_group_raii {
public:
	settings_group_raii(QSettings &settings, const QString &key)
			: m_settings(settings) {
		m_settings.beginGroup(key);
	}

	settings_group_raii(QSettings &settings, const std::string &key)
			: settings_group_raii(settings, QString::fromStdString(key)) {}

	settings_group_raii(QSettings &settings, const char *key)
			: settings_group_raii(settings, QString::fromUtf8(key)) {}

	~settings_group_raii() {
		m_settings.endGroup();
	}

private:
	QSettings &m_settings;
};
