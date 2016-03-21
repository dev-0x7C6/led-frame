#include "alc-screen-widget.h"
#include "ui_alc-screen-widget.h"

#include "classes/alc-color-samples.h"
#include "dialogs/alc-screen-configure-dialog.h"
#include "emitters/alc-screen-emitter.h"
#include "emitters/alc-screen-emitter.h"
#include "managers/alc-emitter-manager.h"
#include "widgets/alc-emitter-widget.h"

ALCScreenWidget::ALCScreenWidget(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ALCScreenWidget) {
	QPalette p = palette();
	p.setColor(QPalette::AlternateBase, QColor(226, 237, 253));
	setPalette(p);
	ui->setupUi(this);
	setup();
	ui->tree->setAlternatingRowColors(true);
	ui->tree->setColumnCount(4);
	ui->tree->header()->setStretchLastSection(false);
	ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
	ui->tree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
}

ALCScreenWidget::~ALCScreenWidget() {
	delete ui;
}

void ALCScreenWidget::setup() {
	ui->tree->clear();
	QListIterator <Emitters::ALCEmitter *> ii(ALCEmitterManager::instance()->allEmitters());
	Emitters::ALCEmitter *emitter;

	while (ii.hasNext()) {
		switch ((emitter = ii.next())->type()) {
			case Emitters::ALCEmitter::Type::ScreenCapture:
				insertScreenCaptureItem(emitter);
				break;

			default:
				break;
		}
	}
}

void ALCScreenWidget::insertScreenCaptureItem(Emitters::ALCEmitter *ptr) {
	Emitters::ALCScreenEmitter *emitter = dynamic_cast <Emitters::ALCScreenEmitter *>(ptr);
	QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
	item->setText(0, emitter->emitterName());
	emitter->setTreeItem(item);
	item->setIcon(0, QIcon(":/22x22/screen.png"));
	QPushButtonEx *button = new QPushButtonEx();
	button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	button->setEmitter(emitter);
	button->setIcon(QIcon(":/16x16/configure.png"));
	button->setFlat(true);
	button->setToolTip(emitter->emitterName());
	connect(button, &QPushButtonEx::clicked, this, &ALCScreenWidget::configure);
	ui->tree->setItemWidget(item, 2, button);
	button = new QPushButtonEx();
	button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	button->setEmitter(emitter);
	button->setIcon(QIcon(":/16x16/rename.png"));
	button->setToolTip(emitter->emitterName());
	button->setFlat(true);
	button->setEnabled(false);
	connect(button, &QPushButtonEx::clicked, this, &ALCScreenWidget::configure);
	ui->tree->setItemWidget(item, 3, button);
}

void ALCScreenWidget::configure() {
	ALCScreenConfigureDialog dialog;
	dialog.setWindowTitle(reinterpret_cast<QPushButtonEx *>(sender())->toolTip());
	dialog.setEmitter(dynamic_cast<Emitters::ALCScreenEmitter *>(reinterpret_cast<QPushButtonEx *>(sender())->emitter()));
	dialog.exec();
}




