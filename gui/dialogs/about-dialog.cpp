#include <gui/dialogs/about-dialog.h>
#include <ui_about-dialog.h>

using namespace Widget;

AboutDialog::AboutDialog(QWidget *parent) :
	DecoratedDialog(parent),
	m_ui(new Ui::AboutDialog)

{
	wave().setAnimationEnabled(true);
	m_ui->setupUi(this);
	m_ui->text->setText(m_ui->text->text().arg(QApplication::applicationDisplayName()));
}

AboutDialog::~AboutDialog() {
	delete m_ui;
}
