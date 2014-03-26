#include "alc-emitters-widget.h"
#include "ui_alc-emitters-widget.h"

#include "classes/color-samples.h"
#include "emitters/color-emitter.h"
#include "emitters/screen-capture-color-emitter.h"
#include "managers/alc-emitter-manager.h"

ALCEmittersWidget::ALCEmittersWidget(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ALCEmittersWidget) {
  ui->setupUi(this);
  setup();
  ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
}

ALCEmittersWidget::~ALCEmittersWidget() {
  delete ui;
}


void ALCEmittersWidget::addPlainColorItem() {
  ALCEmitterManager::instance()->addPlainColorEmitter();
}

void ALCEmittersWidget::addAnimationItem() {
  ALCEmitterManager::instance()->addAnimationColorEmitter();
}

void ALCEmittersWidget::addImageItem() {
  ALCEmitterManager::instance()->addImageColorEmitter();
}

void ALCEmittersWidget::prepare() {
  setup();
}


void ALCEmittersWidget::setup() {
  ui->tree->clear();
  QListIterator < ColorEmitter*> ii(ALCEmitterManager::instance()->allEmitters());

  ColorEmitter *emitter;
  while (ii.hasNext()) {

    switch ((emitter = ii.next())->type()) {
    case ColorEmitter::EMITTER_PLAIN_COLOR:
      insertPlainColorItem(emitter);
      break;
    case ColorEmitter::EMITTER_ANIMATION:
      insertAnimationItem(emitter);
      break;
    case ColorEmitter::EMITTER_IMAGE:
      insertImageItem(emitter);
      break;
    default:
      break;
    }
  }
}

#include "emitters/plain-color-emitter.h"
#include "emitters/animation-color-emitter.h"
#include "emitters/image-color-emitter.h"

#include <QPushButton>

void ALCEmittersWidget::insertPlainColorItem(ColorEmitter *ptr) {
  PlainColorEmitter *emitter = dynamic_cast < PlainColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/color.png"));
  QPushButtonEx *button = new QPushButtonEx();
  button->setEmitter(emitter);
  prepareColorButton(button, emitter->color());
  ui->tree->setItemWidget(item, 1, button);
  connect(button, &QPushButtonEx::clicked, this, &ALCEmittersWidget::pickColor);
}

void ALCEmittersWidget::insertAnimationItem(ColorEmitter *ptr) {
  AnimationColorEmitter *emitter = dynamic_cast < AnimationColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/animation.png"));
  QPushButtonEx *button = new QPushButtonEx();
  button->setEmitter(emitter);
  button->setText("configure animation...");
  ui->tree->setItemWidget(item, 1, button);
 // connect(button, &QPushButtonEx::clicked, this, &ALCEmittersWidget::pickColor);
}

void ALCEmittersWidget::insertImageItem(ColorEmitter *ptr) {
  ImageColorEmitter *emitter = dynamic_cast < ImageColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/from-image.png"));
  QPushButtonEx *button = new QPushButtonEx();
  button->setEmitter(emitter);
  button->setText((emitter->file().isEmpty()) ? "load samples from image..." : emitter->file());
  ui->tree->setItemWidget(item, 1, button);

  connect(button, &QPushButtonEx::clicked, this, &ALCEmittersWidget::pickImage);

}

void ALCEmittersWidget::pickImage() {
  QPushButtonEx *button = reinterpret_cast < QPushButtonEx*>( sender());
  ImageColorEmitter *emitter = dynamic_cast < ImageColorEmitter*> ( button->emitter());
  QString fileName = emitter->open();
  button->setText((fileName.isEmpty()) ? "load samples from image..." : fileName);
}

#include <QPainter>

void ALCEmittersWidget::pickColor() {
  QPushButtonEx *button = reinterpret_cast < QPushButtonEx*>( sender());
  PlainColorEmitter *emitter = dynamic_cast < PlainColorEmitter*> ( button->emitter());

  QColor color = emitter->open();
  prepareColorButton(button, color);
}

void ALCEmittersWidget::prepareColorButton(QPushButton *button, QColor color) {
  button->setText(QString("  RGB (%1; %2; %3)").arg(
                    QString::number(color.red()),
                    QString::number(color.green()),
                    QString::number(color.blue())));

  QPixmap pixmap(QSize(16, 16));
  QPainter painter(&pixmap);
  painter.setBrush(color);
  painter.setPen(color);
  painter.drawRect(pixmap.rect());
  button->setIcon(QIcon(pixmap));
}


