#include <gui/dialogs/about-dialog.h>
#include <ui_about-dialog.h>

#include <QColor>

using namespace Widget;

AboutDialog::AboutDialog(QWidget *parent) :
	DecoratedDialog(parent),
	m_ui(new Ui::AboutDialog)

{
	wave().setAnimationEnabled(true);
	m_ui->setupUi(this);
	m_ui->text->setText(m_ui->text->text().arg(QApplication::applicationDisplayName()));
	startTimer(1000 / 30);
}

AboutDialog::~AboutDialog() {
	delete m_ui;
}

QString AboutDialog::name() const {
	return "About";
}

Enum::EmitterType AboutDialog::type() const {
	return Enum::EmitterType::About;
}

void AboutDialog::timerEvent(QTimerEvent *) {
	commit(wave().scanline());
}

void AboutDialog::onConnect(const uint32_t &) {
}

void AboutDialog::onDisconnect(const uint32_t &) {
}
