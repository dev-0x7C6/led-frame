#include "corrector-settings-dialog.h"

#include <QSlider>
#include <QHBoxLayout>

using namespace Widget;

CorrectorSettingsDialog::CorrectorSettingsDialog(double factor, QWidget *parent)
	: QDialog(parent)
{
	auto layout = new QHBoxLayout;
	auto slider = new QSlider(Qt::Horizontal, this);
	slider->setMinimum(min());
	slider->setMaximum(max());
	slider->setTickInterval(slider->maximum() / tickCount());
	slider->setMinimumWidth(600);
	slider->setMinimumHeight(100);
	slider->setValue(static_cast<int>(factor * max()));
	slider->setTickPosition(QSlider::TicksBelow);
	layout->addWidget(slider);
	setLayout(layout);
	connect(slider, &QSlider::valueChanged, this, &CorrectorSettingsDialog::valueChanged);
}

