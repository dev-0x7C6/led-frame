#ifndef AMBIENTLIGHTSYMULATION_H
#define AMBIENTLIGHTSYMULATION_H

#include "emitters/color-emitter.h"

#include <QGLWidget>
#include <QRgb>

class AmbientLightSymulation :public QGLWidget
{
  Q_OBJECT
private:
  QList< QRgb> colors;
  QPixmap m_monitor;
  ColorEmitter *m_emitter;

  int z;

  int m_timerId;
  int m_framerateLimit;
  int m_glowSize;

public:
  explicit AmbientLightSymulation(QWidget *parent = 0);
  ~AmbientLightSymulation() {
    if (m_emitter)
      m_emitter->done();
    z = 100000;
  }


protected:

  void timerEvent(QTimerEvent *) {
   // glDraw();
  }

    void initializeGL()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0, 0.0, 0.0, 0.0);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       // glDisable(GL_DEPTH_TEST);

    }

    void resizeGL(int w, int h) {
        glViewport(0, 0, (GLint)w, (GLint)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

    }


    void drawGlowArea(float x, float y, float r, float *color) {
      glBegin(GL_TRIANGLE_FAN);
      glColor4fv(color);
      glVertex3f(x, y, 0.5);
      glColor4f(0, 0, 0, 0);
      for (register double i = 0; i <= 2 * M_PI; i += M_PI / 24)
        glVertex3f(x + sin(i) * r, y + cos(i) * r, 0);
      glEnd();
    }

    void paintGL()
    {
      glDepthMask(true);

      float rgba[4];
      rgba[0] = 0.0f;
      rgba[1] = 1.0f;
      rgba[2] = 0.0f;
      rgba[3] = 0.0f;
      drawGlowArea(0.0f, 0.0f, 0.4f, rgba);
      rgba[0] = 1.0f;
      rgba[1] = 0.0f;
      drawGlowArea(0.4f, 0.0f, 0.4f, rgba);

    }


public slots:
  void setFramerate(int);
  void setGlowSize(int);

  void connectEmitter(ColorEmitter *emitter) {
    if (m_emitter)
      m_emitter->done();

    (m_emitter = emitter)->init();
  }


private slots:

protected:
  void paintEvent(QPaintEvent *);
};

#endif // AMBIENTLIGHTSYMULATION_H
