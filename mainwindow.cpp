#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  timerId(-1),
  m_brightness(1.0)
{
  ui->setupUi(this);
 // connect(this, SIGNAL(update()), this, SLOT(screenUpdate()), Qt::QueuedConnection);
  emit update();

  QString info = " x:" +
      QString::number(QApplication::desktop()->geometry().x()) + ", y:" +
      QString::number(QApplication::desktop()->geometry().y()) + " (" +
      QString::number(QApplication::desktop()->geometry().width()) + "x" +
      QString::number(QApplication::desktop()->geometry().height()) + ")";
  ui->comboBox->addItem(QIcon(":/16x16/all-screens.png"), "Visible area, " + info, -1);
  for (register int i = 0; i < QApplication::desktop()->screenCount(); ++i) {
    info = " x:" +
        QString::number(QApplication::desktop()->screenGeometry(i).x()) + ", y:" +
        QString::number(QApplication::desktop()->screenGeometry(i).y()) + " (" +
        QString::number(QApplication::desktop()->screenGeometry(i).width()) + "x" +
        QString::number(QApplication::desktop()->screenGeometry(i).height()) + ")";
    ui->comboBox->addItem(QIcon(":/16x16/selected-screen.png"), "Screen " + QString::number(i) + ": "  + info, i);
  }

  updateScreenArea(0);

  connect(this, SIGNAL(updateLeds(QList<QRgb>)), ui->widget, SLOT(updateLeds(QList<QRgb>)));

  setFramerate(30);

  connect(ui->brightnessSlider_2, SIGNAL(valueChanged(int)), this, SLOT(setFramerate(int)));
  connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
  connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenArea(int)));
}

#include <QPixmap>
#include <QDebug>

void MainWindow::timerEvent(QTimerEvent *) {
  screenUpdate();
}

void MainWindow::setFramerate(int) {
  if (timerId != -1)
    killTimer(timerId);
  timerId = startTimer(1000/ui->brightnessSlider_2->value());
  ui->fpsCount->setText(QString::number(ui->brightnessSlider_2->value()) + "fps");
}

#include <QPainter>
#include <QRgb>

#include <QElapsedTimer>

const int pixelSkip = 8;

void MainWindow::setBrightness(int value) {
  m_brightness = double(value) / 100.0;
  ui->proc->setText(QString::number(int(m_brightness*100)) + "%");
}


void MainWindow:: updateScreenArea(int area) {
  QRect geometry;
  if (area == 0) {
    geometry = QApplication::desktop()->geometry();
  } else {
    geometry = QApplication::desktop()->screenGeometry(area-1);
  }

  ui->spinBox->setValue(0);
  ui->spinBox_2->setValue(0);
  ui->spinBox_3->setValue(geometry.width());
  ui->spinBox_4->setValue(geometry.height());
}

#include <QScreen>

void MainWindow::screenUpdate() {
  QElapsedTimer timer;
  timer.start();
  int screen = ui->comboBox->itemData(ui->comboBox->currentIndex(), Qt::UserRole).toInt();

  QRect rect = QApplication::desktop()->screenGeometry(screen);


  int alghoritm = ui->alghoritm->currentIndex();


 //qDebug() << QApplication::desktop()->winId();
  rect.setCoords(ui->spinBox->value() + rect.x(), ui->spinBox_2->value() + rect.y(), ui->spinBox_3->value() + rect.x(), ui->spinBox_4->value() + rect.y());
  //qDebug() << rect;

  WId wid = 0;


  switch (alghoritm) {
  case 0:
    m_wholeScreen = QApplication::screens().first()->grabWindow(wid, rect.x(), rect.y(), rect.width(), rect.height());
    break;
  case 1:
    m_screenChunk[0] = QApplication::screens().first()->grabWindow(wid, rect.x(), rect.y(), rect.width(), 48);
    m_screenChunk[1] = QApplication::screens().first()->grabWindow(wid, rect.x(), rect.y() + rect.height() - 48, rect.width(), 48);
    m_screenChunk[2] = QApplication::screens().first()->grabWindow(wid, rect.x(), rect.y(), 48, rect.height());
    m_screenChunk[3] = QApplication::screens().first()->grabWindow(wid, rect.x() + rect.width() - 48, rect.y(), 48, rect.height());
    break;
  case 2:
    break;
  }


  QList< QRgb> colors;
  quint32 vrgb[3];
  QImage img;

  for (register int context = 0; context < 4; ++context) {
    for (register int i = 0; i < 8; ++i) {
      memset(reinterpret_cast< void*>(vrgb), 0, sizeof(vrgb));

      switch (alghoritm) {
      case 0:
        switch (context) {
        case 0:
          img = m_wholeScreen.copy(i * (rect.width()/8), 0, 48, 48).toImage();
          break;
        case 1:
          img = m_wholeScreen.copy(i * (rect.width()/8), rect.height()-48, 48 , 48).toImage();
          break;
        case 2:
          img = m_wholeScreen.copy(0, i * (rect.height()/8), 48 , 48).toImage();
          break;
        case 3:
          img = m_wholeScreen.copy(rect.width()-48, i * (rect.height()/8), 48, 48).toImage();
          break;
        }
        break;
      case 1:
        switch (context) {
        case 0:
        case 1:
          img = m_screenChunk[context].copy(i * rect.width() / 8, 0, 48, 48).toImage();
          break;
        case 2:
        case 3:
          img = m_screenChunk[context].copy(0, i * rect.height() / 8, 48, 48).toImage();
          break;
        }
        break;
      case 2:
        switch (context) {
        case 0:
          img = QApplication::screens().first()->grabWindow(wid, rect.x() + i * (rect.width()/8), rect.y(), 48, 48).toImage();
          break;
        case 1:
          img = QApplication::screens().first()->grabWindow(wid, rect.x() + i * (rect.width()/8), rect.y()+rect.height()-48, 48, 48).toImage();
          break;
        case 2:
          img = QApplication::screens().first()->grabWindow(wid, rect.x(), rect.y() + i * (rect.height()/8), 48, 48).toImage();
          break;
        case 3:
          img = QApplication::screens().first()->grabWindow(wid, rect.x() + rect.width() - 48, rect.y() + i * (rect.height()/8), 48, 48).toImage();
          break;
        }
        break;
      }

      quint32 *source = reinterpret_cast< quint32*>(img.bits());
      int length = img.byteCount()/img.depth();
      for (register int i = 0; i < length; i += pixelSkip) {
        vrgb[0] += uchar(source[i] >> 16);
        vrgb[1] += uchar(source[i] >> 8);
        vrgb[2] += uchar(source[i]);
      }

      vrgb[0] = double(vrgb[0]) / double(length/(pixelSkip * m_brightness));
      vrgb[1] = double(vrgb[1]) / double(length/(pixelSkip * m_brightness));
      vrgb[2] = double(vrgb[2]) / double(length/(pixelSkip * m_brightness));

      for (register int c = 0; c < 3; ++c)
        if (vrgb[c] > 255) vrgb[c] = 255;

      colors << qRgb(vrgb[0], vrgb[1], vrgb[2]);
    }
  }

  qDebug() << "Chuks prepared in " << timer.elapsed() << "ms";

  emit updateLeds(colors);

}

MainWindow::~MainWindow()
{
  delete ui;
}
