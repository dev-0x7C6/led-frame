#include "alc-screens-widget.h"
#include "ui_alc-screens-widget.h"

#include "classes/color-samples.h"
#include "emitters/screen-capture-color-emitter.h"
#include "managers/alc-emitter-manager.h"

#include "widgets/alc-emitters-widget.h"

ALCScreensWidget::ALCScreensWidget(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ALCScreensWidget) {
  ui->setupUi(this);
  setup();
  ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
}

ALCScreensWidget::~ALCScreensWidget() {
  delete ui;
}

//#include <QListIterator>

#include <QDebug>

void ALCScreensWidget::setup() {
  ui->tree->clear();
  QListIterator < ColorEmitter*> ii(ALCEmitterManager::instance()->allEmitters());

  ColorEmitter *emitter;
  while (ii.hasNext()) {

    switch ((emitter = ii.next())->type()) {
    case ColorEmitter::EMITTER_SCREEN_CAPTURE:
      insertScreenCaptureItem(emitter);
      break;
    default:
      break;
    }
  }
}

void ALCScreensWidget::insertScreenCaptureItem(ColorEmitter *ptr) {
  ScreenCaptureColorEmitter *emitter = dynamic_cast < ScreenCaptureColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/screen.png"));
  QPushButtonEx *button = new QPushButtonEx();
  button->setEmitter(emitter);
  //prepareColorButton(button, emitter->color());
  button->setText("configure...");
  ui->tree->setItemWidget(item, 1, button);
  connect(button, &QPushButtonEx::clicked, this, &ALCScreensWidget::configure);
}

#include "dialogs/alc-screen-configure-dialog.h"

void ALCScreensWidget::configure() {
  ALCScreenConfigureDialog dialog;
  dialog.exec();
}

//void ALCScreensWidget::insertScreenCaptureItem(ColorEmitter *)
//{

//}

#include "emitters/plain-color-emitter.h"
#include "emitters/animation-color-emitter.h"
#include "emitters/image-color-emitter.h"

#include <QPushButton>

//void ALCScreensWidget::insertPlainColorItem(ColorEmitter *ptr) {
//  PlainColorEmitter *emitter = dynamic_cast < PlainColorEmitter*> ( ptr);
//  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
//  item->setText(0, emitter->emitterName());
//  emitter->setTreeItem(item);
//  item->setIcon(0, QIcon(":/22x22/color.png"));
//  QPushButtonEx *button = new QPushButtonEx();
//  button->setEmitter(emitter);
//  prepareColorButton(button, emitter->color());
//  ui->tree->setItemWidget(item, 1, button);
//  connect(button, &QPushButtonEx::clicked, this, &ALCScreensWidget::pickColor);
//}


