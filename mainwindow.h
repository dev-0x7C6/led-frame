#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScreen>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
  QPixmap m_wholeScreen;
  int timerId;
  double m_brightness;
  QPixmap m_screenChunk[4];

public:
  enum {
    ScreenFragmentsTop = 0,
    ScreenFragmentsButtom,
    ScreenFragmentsLeft,
    ScreenFragmentsRight
  };

explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void timerEvent(QTimerEvent *);

private slots:
  void screenUpdate();
  void setFramerate(int);
  void setBrightness(int value);
  void updateScreenArea(int);
  
private:
  Ui::MainWindow *ui;

signals:
  void update();
  void updateLeds(QList< QRgb> colors);
};

#endif // MAINWINDOW_H

/*
void MainWindow::screenUpdate() {
  QElapsedTimer timer;
  timer.start();
  int screen = ui->comboBox->itemData(ui->comboBox->currentIndex(), Qt::UserRole).toInt();

  QDesktopWidget *desktop = QApplication::desktop();
  QRect rect = desktop->screenGeometry(screen);

  QPixmap m_screenChunk[4];
 /// m_wholeScreen = QPixmap::grabWindow(QApplication::desktop()->winId(), rect.x(), rect.y(), rect.width(), rect.height());


  m_screenChunk[0] = QPixmap::grabWindow(QApplication::desktop()->winId(), rect.x(), rect.y(), rect.width(), 48);
  m_screenChunk[1] = QPixmap::grabWindow(QApplication::desktop()->winId(), rect.x(), rect.y() + rect.height() - 48, rect.width(), 48);
  m_screenChunk[2] = QPixmap::grabWindow(QApplication::desktop()->winId(), rect.x(), rect.y(), 48, rect.height());
  m_screenChunk[3] = QPixmap::grabWindow(QApplication::desktop()->winId(), rect.x() + rect.width() - 48, rect.y(), 48, rect.height());
  QList< QRgb> colors;
  quint32 vrgb[3];
  QImage img;

  for (register int context = 0; context < 4; ++context) {
    for (register int i = 0; i < 8; ++i) {
      memset(reinterpret_cast< void*>(vrgb), 0, sizeof(vrgb));
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

      quint32 *source = reinterpret_cast< quint32*>(img.bits());
      int length = img.byteCount()/img.depth();
      for (register int i = 0; i < length; i += 8) {
        vrgb[0] += uchar(source[i] >> 16);
        vrgb[1] += uchar(source[i] >> 8);
        vrgb[2] += uchar(source[i]);
      }
      vrgb[0] = double(vrgb[0]) / double(length/8);
      vrgb[1] = double(vrgb[1]) / double(length/8);
      vrgb[2] = double(vrgb[2]) / double(length/8);
      colors << qRgb(vrgb[0], vrgb[1], vrgb[2]);
    }
  }

 */
