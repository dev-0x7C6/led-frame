#pragma once

#include <QDialog>

namespace Widget {

class CorrectorSettingsDialog final : public QDialog {
	Q_OBJECT
public:
	explicit CorrectorSettingsDialog(double factor = 0, QWidget *parent = nullptr);
	virtual ~CorrectorSettingsDialog() = default;

	constexpr static int min() { return 0; }
	constexpr static int max() { return 1000000; }
	constexpr static int tickCount() { return 20; }

	constexpr static double factor(int value) { return static_cast<double>(value) / static_cast<double>(max()); }

signals:
	void valueChanged(int value);
};
}
