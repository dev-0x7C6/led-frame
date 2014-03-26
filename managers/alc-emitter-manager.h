#ifndef ALCEmitterManager_H
#define ALCEmitterManager_H

#include <QObject>

#include "emitters/color-emitter.h"

class ColorEmitter;
class ImageColorEmitter;
class PlainColorEmitter;
class AnimationColorEmitter;
class ScreenCaptureColorEmitter;

class ALCEmitterManager : public QObject {
  Q_OBJECT
private:
  QList < ColorEmitter *> m_emitters[ColorEmitter::EMITTER_END_ARRAY];

public:
  explicit ALCEmitterManager(QObject *parent = 0);
  virtual ~ALCEmitterManager();

  void add(ColorEmitter *, ColorEmitter::EmitterType type);
  ImageColorEmitter *addImageColorEmitter();
  PlainColorEmitter *addPlainColorEmitter();
  AnimationColorEmitter *addAnimationColorEmitter();
  ScreenCaptureColorEmitter *addScreenCaptureEmitter();

  void remove(ColorEmitter *);

  static ALCEmitterManager* instance() {
    static ALCEmitterManager object;
    return &object;
  }

  QList < ColorEmitter *> *emitters(ColorEmitter::EmitterType type);
  QList < ColorEmitter *> allEmitters();

signals:
  void emitterListChanged();

};



#endif // ALCEmitterManager_H
