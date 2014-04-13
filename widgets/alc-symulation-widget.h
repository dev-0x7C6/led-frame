#ifndef AMBIENTLIGHTSYMULATION_H
#define AMBIENTLIGHTSYMULATION_H

#include "emitters/color-emitter.h"
#include "classes/alc-color-samples.h"


#include <QWidget>
#include <QRgb>

#include <QVector>

class QQuickView;
class QQuickItem;

class ALCSymulationWidget :public QWidget {
  Q_OBJECT
private:
  ALCColorSamples m_samples;
  ColorEmitter *m_emitter;
  QQuickItem *m_root;

  QQuickItem *m_items[4][8];
  QQuickItem *m_monitor;
  QQuickView *m_view;

public:
  explicit ALCSymulationWidget(QWidget *parent = 0);
  virtual ~ALCSymulationWidget();

  void onShow();

  ColorEmitter *connectedEmitter() const;
  void connectEmitter(ColorEmitter *emitter);

  void createQmlMonitor();
  void freeQmlMonitor();
  void createQmlObjects(int size = 300);
  void freeQmlObjects();
  void resetQmlObjects();

private:
  void createQmlObject(int ii, int i, QQuickItem *item, int size);

protected:
  void timerEvent(QTimerEvent *);
  void showEvent(QShowEvent *);
};

#endif // AMBIENTLIGHTSYMULATION_H
