#include "alc-selections-widget.h"
#include "ui_alc-selections-widget.h"

#include "classes/alc-color-samples.h"
#include "emitters/screen-capture-color-emitter.h"
#include "managers/alc-emitter-manager.h"

#include "widgets/alc-emitters-widget.h"

ALCSelectionsWidget::ALCSelectionsWidget(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ALCSelectionsWidget) {
  ui->setupUi(this);
  setup();
  ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
}

ALCSelectionsWidget::~ALCSelectionsWidget() {
  delete ui;
}

//#include <QListIterator>

#include <QDebug>

void ALCSelectionsWidget::setup() {
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

void ALCSelectionsWidget::insertScreenCaptureItem(ColorEmitter *ptr) {
  ScreenCaptureColorEmitter *emitter = dynamic_cast < ScreenCaptureColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/screen.png"));
  QPushButtonEx *button = new QPushButtonEx();
  button->setEmitter(emitter);
  //prepareColorButton(button, emitter->color());
  ui->tree->setItemWidget(item, 1, button);
//  connect(button, &QPushButtonEx::clicked, this, &ALCSelectionsWidget::pickColor);
}

//void ALCSelectionsWidget::insertScreenCaptureItem(ColorEmitter *)
//{

//}

#include "emitters/plain-color-emitter.h"
#include "emitters/animation-color-emitter.h"
#include "emitters/image-color-emitter.h"

#include <QPushButton>

//void ALCSelectionsWidget::insertPlainColorItem(ColorEmitter *ptr) {
//  PlainColorEmitter *emitter = dynamic_cast < PlainColorEmitter*> ( ptr);
//  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
//  item->setText(0, emitter->emitterName());
//  emitter->setTreeItem(item);
//  item->setIcon(0, QIcon(":/22x22/color.png"));
//  QPushButtonEx *button = new QPushButtonEx();
//  button->setEmitter(emitter);
//  prepareColorButton(button, emitter->color());
//  ui->tree->setItemWidget(item, 1, button);
//  connect(button, &QPushButtonEx::clicked, this, &ALCSelectionsWidget::pickColor);
//}


