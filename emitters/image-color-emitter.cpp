#include "emitters/image-color-emitter.h"

#include <QFileInfo>
#include <QFileDialog>

ImageColorEmitter::ImageColorEmitter(QObject *parent) :
  QObject(parent),
  ColorEmitter(),
  m_image(0),
  m_timer(new QTimer())
{
  m_type = EMITTER_IMAGE;
  connect(m_timer, &QTimer::timeout, this, &ImageColorEmitter::pushState);
  m_timer->setInterval(20);
  m_timer->start();


}

void ImageColorEmitter::fromImage(QImage *image) {
  m_image = image;
  QVector < QImage> fragments[4];

  fragments[0].resize(64);
  fragments[1].resize(64);
  fragments[2].resize(64);
  fragments[3].resize(64);

  for (register int i = 0; i < 64; ++i)
    fragments[0][i] = image->copy((image->width()/64)*i, 0, (image->width()/64), 200);
  for (register int i = 0; i < 64; ++i)
    fragments[1][i] = image->copy((image->width()/64)*i, image->height() - 200, (image->width()/64), 200);
  for (register int i = 0; i < 64; ++i)
    fragments[2][i] = image->copy((image->width()/64)*i, image->height() - 200, (image->width()/64), 200);
  for (register int i = 0; i < 64; ++i)
    fragments[3][i] = image->copy((image->width()/64)*i, image->height() - 200, (image->width()/64), 200);



  int c = 0;
  double r = 0;
  double g = 0;
  double b = 0;
  int rgb = 0;

  for (register int k = 0; k < 4; ++k) {
    QVector < int> *colors = m_samples.get(ColorSamples::Position(k));
    for (register int i = 0; i < SAMPLE_RESOLUTION; ++i) {
      r = g = b = 0;
      c = 0;
      for (register int x = 0; x < fragments[k][i].width(); ++x)
      for (register int y = 0; y < fragments[k][i].height(); ++y) {
        rgb = fragments[k][i].pixel(x, y);
        r += qRed(rgb);
        g += qGreen(rgb);
        b += qBlue(rgb);
        c++;
      }

      r /= c;
      g /= c;
      b /= c;

      if (r > 255) r = 255;
      if (g > 255) g = 255;
      if (b > 255) b = 255;

      (*colors)[i] = qRgb(r, g, b);
    }
  }
}

void ImageColorEmitter::pushState() {
  setState(m_samples);
}

void ImageColorEmitter::init() {
  QMetaObject::invokeMethod(this, "open", Qt::QueuedConnection);
  ColorEmitter::init();
}

void ImageColorEmitter::open()
{
  QFileInfo info(QFileDialog::getOpenFileName(0,
                                              "Open Keylogger History File",
                                              "*.jpg;*.png", "Images (*.png *.xpm *.jpg)"));

  if (info.exists() && info.isFile() && info.isReadable()) {
    fromImage(new QImage(info.filePath()));
  }
}
