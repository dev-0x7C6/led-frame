#include "alc-emitters-widget.h"
#include "ui_alc-emitters-widget.h"

#include <QInputDialog>
#include <QPainter>
#include <QPushButton>

#include "classes/alc-color-samples.h"
#include "emitters/animation-color-emitter.h"
#include "emitters/color-emitter.h"
#include "emitters/image-color-emitter.h"
#include "emitters/plain-color-emitter.h"
#include "emitters/screen-capture-color-emitter.h"
#include "managers/alc-emitter-manager.h"

#include <QPalette>

ALCEmittersWidget::ALCEmittersWidget(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::ALCEmittersWidget) {
  QPalette p = palette();
  p.setColor( QPalette::AlternateBase, QColor(226, 237, 253) );
  setPalette(p);

  ui->setupUi(this);
  setup();


  ui->tree->setColumnCount(3);
  ui->tree->header()->setStretchLastSection(false);
  ui->tree->header()->resizeSections(QHeaderView::ResizeToContents);
  ui->tree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
}

ALCEmittersWidget::~ALCEmittersWidget() {
  delete ui;
}

void ALCEmittersWidget::addPlainColorItem() {
  QString input = QInputDialog::getText(this, "Name", "Get name:");
  if (!input.isEmpty())
    ALCEmitterManager::instance()->addPlainColorEmitter(input);
}

void ALCEmittersWidget::addAnimationItem() {
  QString input = QInputDialog::getText(this, "Name", "Get name:");
  if (!input.isEmpty())
    ALCEmitterManager::instance()->addAnimationColorEmitter(input);
}

void ALCEmittersWidget::addImageItem() {
  QString input = QInputDialog::getText(this, "Name", "Get name:");
  if (!input.isEmpty())
    ALCEmitterManager::instance()->addImageColorEmitter(input);
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

void ALCEmittersWidget::insertPlainColorItem(ColorEmitter *ptr) {
  PlainColorEmitter *emitter = dynamic_cast < PlainColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/color.png"));


  QWidget *workspace = new QWidget();
  QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
  workspace->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  workspace->setLayout(layout);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  insertDefaultButtons(emitter, layout);


  ui->tree->setItemWidget(item, 2, workspace);
  prepareColorItem(item, emitter->color());
}

void ALCEmittersWidget::insertAnimationItem(ColorEmitter *ptr) {
  AnimationColorEmitter *emitter = dynamic_cast < AnimationColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());

  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/animation.png"));

  QWidget *workspace = new QWidget();
  QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
  workspace->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  workspace->setLayout(layout);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  insertDefaultButtons(emitter, layout);


  ui->tree->setItemWidget(item, 2, workspace);
}

void ALCEmittersWidget::insertImageItem(ColorEmitter *ptr) {
  ImageColorEmitter *emitter = dynamic_cast < ImageColorEmitter*> ( ptr);
  QTreeWidgetItem *item = new QTreeWidgetItem(ui->tree);
  item->setText(0, emitter->emitterName());
  item->setText(1, emitter->file());
  item->setTextColor(1, Qt::darkGray);
  emitter->setTreeItem(item);
  item->setIcon(0, QIcon(":/22x22/from-image.png"));

  QWidget *workspace = new QWidget();
  QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
  workspace->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  workspace->setLayout(layout);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  insertDefaultButtons(emitter, layout);


  ui->tree->setItemWidget(item, 2, workspace);

}

void ALCEmittersWidget::insertDefaultButtons(ColorEmitter *emitter, QBoxLayout *layout) {
  QPushButtonEx *configure = new QPushButtonEx;
  QPushButtonEx *rename = new QPushButtonEx;
  QPushButtonEx *remove = new QPushButtonEx;

  configure->setIcon(QIcon(":/16x16/configure.png"));
  configure->setFlat(true);
  configure->setEmitter(emitter);

  rename->setIcon(QIcon(":/16x16/rename.png"));
  rename->setFlat(true);
  rename->setEmitter(emitter);

  remove->setIcon(QIcon(":/16x16/edit-delete.png"));
  remove->setFlat(true);
  remove->setEmitter(emitter);


  connect(configure, &QPushButtonEx::clicked, this, &ALCEmittersWidget::reconfigure);
  connect(remove, &QPushButtonEx::clicked, this, &ALCEmittersWidget::remove);
  connect(rename, &QPushButtonEx::clicked, this, &ALCEmittersWidget::rename);

  layout->addWidget(configure);
  layout->addWidget(rename);
  layout->addWidget(remove);
}

void ALCEmittersWidget::reconfigure() {
  reinterpret_cast < QPushButtonEx*>( sender())->emitter()->configure();
  setup();
}

void ALCEmittersWidget::rename() {
  reinterpret_cast < QPushButtonEx*>( sender())->emitter()->rename();
  setup();
}

void ALCEmittersWidget::remove() {
  reinterpret_cast < QPushButtonEx*>( sender())->emitter()->remove();
  setup();
}

void ALCEmittersWidget::prepareColorItem(QTreeWidgetItem *item, QColor color) {
  item->setTextColor(1, Qt::darkGray);
  item->setText(1, QString("  RGB (%1; %2; %3)").arg(
                   QString::number(color.red()),
                   QString::number(color.green()),
                   QString::number(color.blue())));

  QPixmap pixmap(QSize(16, 16));
  QPainter painter(&pixmap);
  painter.setBrush(color);
  painter.setPen(Qt::darkGray);
  painter.drawRect(0, 0, 15, 15);
  item->setIcon(1, QIcon(pixmap));
}


