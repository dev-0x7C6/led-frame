#ifndef ALCSCREENMANAGER_H
#define ALCSCREENMANAGER_H

#include <QObject>

class ScreenCaptureColorEmitter;

class ALCScreenManager : public QObject {
  Q_OBJECT
private:
  QList < ScreenCaptureColorEmitter *> m_screenEmitters;
  QList < ScreenCaptureColorEmitter *> m_screenCustomEmitters;

public:
  explicit ALCScreenManager(QObject *parent = 0);
  virtual ~ALCScreenManager();

  ScreenCaptureColorEmitter *add();
  void remove(ScreenCaptureColorEmitter *emitter);

  QList < ScreenCaptureColorEmitter *> *emitters();
  QList < ScreenCaptureColorEmitter *> *customs();


};

#endif // ALCSCREENMANAGER_H
