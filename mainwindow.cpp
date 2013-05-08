#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  timerId(-1),
  m_brightness(1.0),
  m_totalCycles(0),
  m_totalLatency(0)
{
  ui->setupUi(this);

  QString info = " x:" +
      QString::number(QApplication::desktop()->geometry().x()) + ", y:" +
      QString::number(QApplication::desktop()->geometry().y()) + " (" +
      QString::number(QApplication::desktop()->geometry().width()) + "x" +
      QString::number(QApplication::desktop()->geometry().height()) + ")";
  ui->screenArea->addItem(QIcon(":/16x16/all-screens.png"), "Visible area, " + info, -1);
  for (register int i = 0; i < QApplication::desktop()->screenCount(); ++i) {
    info = " x:" +
        QString::number(QApplication::desktop()->screenGeometry(i).x()) + ", y:" +
        QString::number(QApplication::desktop()->screenGeometry(i).y()) + " (" +
        QString::number(QApplication::desktop()->screenGeometry(i).width()) + "x" +
        QString::number(QApplication::desktop()->screenGeometry(i).height()) + ")";
    ui->screenArea->addItem(QIcon(":/16x16/selected-screen.png"), "Screen " + QString::number(i) + ": "  + info, i);
  }

  updateScreenArea(0);
  setFramerate(30);

  connect(this, SIGNAL(updateLeds(QList<QRgb>)), ui->widget, SLOT(updateLeds(QList<QRgb>)), Qt::DirectConnection);
  connect(ui->framerateLimit, SIGNAL(valueChanged(int)), this, SLOT(setFramerate(int)));
  connect(ui->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
  connect(ui->screenArea, SIGNAL(currentIndexChanged(int)), this, SLOT(updateScreenArea(int)));

  connect(&m_fpsTimer, SIGNAL(timeout()), this, SLOT(calculateFPS()));
  m_fpsTimer.setInterval(1000);
  m_fpsTimer.start();

  //setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::Window);
}

void MainWindow::timerEvent(QTimerEvent *) {
  screenUpdate();
}

void MainWindow::setFramerate(int) {
  if (timerId != -1)
    killTimer(timerId);
  timerId = startTimer(1000.0/double(ui->framerateLimit->value()));
  ui->fpsCount->setText(QString::number(ui->framerateLimit->value()) + "fps");
}

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

  ui->x->setValue(0);
  ui->y->setValue(0);
  ui->xoffset->setValue(geometry.width());
  ui->yoffset->setValue(geometry.height());
}

void MainWindow::calculateFPS() {
  ui->latency->setText("Frame capture latency: " + QString::number(m_totalLatency / m_totalCycles, 'f', 2) + "ms");
  ui->fps->setText("Capture rate: " + QString::number(m_totalCycles) + "fps");
  m_totalCycles = 0;
  m_totalLatency = 0;
}

#include <QDebug>

void MainWindow::screenUpdate() {
  QElapsedTimer timer;
  timer.start();

  int alghoritm = ui->alghoritm->currentIndex();
  int pixelSkip = ui->pixelSkip->value()+1;
  int chunkSize = 32;

  QRect rect = QApplication::desktop()->screenGeometry(ui->screenArea->itemData(ui->screenArea->currentIndex(), Qt::UserRole).toInt());
  rect.setCoords(ui->x->value() + rect.x(), ui->y->value() + rect.y(), ui->xoffset->value() + rect.x() - 1, ui->yoffset->value() + rect.y() - 1);

  QScreen *screen = QGuiApplication::primaryScreen();
  if (!screen) {
    qFatal("shit");
  }

  m_leds.clear();
  switch (alghoritm) {
  case ScreenCaptureFullShots:
    m_wholeScreen = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
    break;
  case ScreenCapturePartialShots:
    m_screenChunk[0] = screen->grabWindow(0, rect.x(), rect.y(), rect.width(), chunkSize);
    m_screenChunk[1] = screen->grabWindow(0, rect.x(), rect.y() + rect.height() - chunkSize, rect.width(), chunkSize);
    m_screenChunk[2] = screen->grabWindow(0, rect.x(), rect.y(), chunkSize, rect.height());
    m_screenChunk[3] = screen->grabWindow(0, rect.x() + rect.width() - chunkSize, rect.y(), chunkSize, rect.height());
    break;
  }

  for (register int context = 0; context < 4; ++context) {
    for (register int i = 0; i < 8; ++i) {
      memset(reinterpret_cast< void*>(m_rgbCache), 0, sizeof(m_rgbCache));

      switch (alghoritm) {
      case ScreenCaptureFullShots:
        switch (context) {
        case 0:
          m_imageCache = m_wholeScreen.copy(i * (rect.width()/8), 0, chunkSize, chunkSize).toImage();
          break;
        case 1:
          m_imageCache = m_wholeScreen.copy(i * (rect.width()/8), rect.height()-chunkSize, chunkSize , chunkSize).toImage();
          break;
        case 2:
          m_imageCache = m_wholeScreen.copy(0, i * (rect.height()/8), chunkSize , chunkSize).toImage();
          break;
        case 3:
          m_imageCache = m_wholeScreen.copy(rect.width()-chunkSize, i * (rect.height()/8), chunkSize, chunkSize).toImage();
          break;
        }
        break;
      case ScreenCapturePartialShots:
        switch (context) {
        case 0:
        case 1:
          m_imageCache = m_screenChunk[context].copy(i * rect.width() / 8, 0, chunkSize, chunkSize).toImage();
          break;
        case 2:
        case 3:
          m_imageCache = m_screenChunk[context].copy(0, i * rect.height() / 8, chunkSize, chunkSize).toImage();
          break;
        }
        break;
      case ScreenCaptureCriticalShots:
        switch (context) {
        case 0:
          m_imageCache = screen->grabWindow(0, rect.x() + i * (rect.width()/8), rect.y(), chunkSize, chunkSize).toImage();
          break;
        case 1:
          m_imageCache = screen->grabWindow(0, rect.x() + i * (rect.width()/8), rect.y()+rect.height()-chunkSize, chunkSize, chunkSize).toImage();
          break;
        case 2:
          m_imageCache = screen->grabWindow(0, rect.x(), rect.y() + i * (rect.height()/8), chunkSize, chunkSize).toImage();
          break;
        case 3:
          m_imageCache = screen->grabWindow(0, rect.x() + rect.width() - chunkSize, rect.y() + i * (rect.height()/8), chunkSize, chunkSize).toImage();
          break;
        }
        break;
      }

      quint32 *source = reinterpret_cast< quint32*>(m_imageCache.bits());
      int length = m_imageCache.byteCount()/m_imageCache.depth();
      for (register int i = 0; i < length; i += pixelSkip) {
        m_rgbCache[0] += uchar(source[i] >> 16);
        m_rgbCache[1] += uchar(source[i] >> 8);
        m_rgbCache[2] += uchar(source[i]);
      }

      m_rgbCache[0] = double(m_rgbCache[0]) / double(length/(pixelSkip * m_brightness));
      m_rgbCache[1] = double(m_rgbCache[1]) / double(length/(pixelSkip * m_brightness));
      m_rgbCache[2] = double(m_rgbCache[2]) / double(length/(pixelSkip * m_brightness));

      for (register int c = 0; c < 3; ++c)
        if (m_rgbCache[c] > 255) m_rgbCache[c] = 255;

      m_leds << qRgb(m_rgbCache[0], m_rgbCache[1], m_rgbCache[2]);
    }
  }

  m_totalCycles++;
  m_totalLatency += timer.elapsed();

  emit updateLeds(m_leds);
}

MainWindow::~MainWindow()
{
  delete ui;
}
