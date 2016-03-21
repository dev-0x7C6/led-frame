#ifndef ALCSCREENEMITTER_H
#define ALCSCREENEMITTER_H

#include "classes/alc-color-samples.h"
#include "emitters/alc-emitter.h"

#include <QList>
#include <QMutex>
#include <QPixmap>
#include <QRect>
#include <QRgb>
#include <QThread>

#include <atomic>

class QScreen;

namespace Emitters {

	enum ScreenFragments {
		Bottom = 0,
		Left,
		Top,
		Right
	};

	class ALCScreenEmitter : public QThread, public ALCEmitter {
		Q_OBJECT
	public:
		explicit ALCScreenEmitter(QObject *parent = 0);
		virtual ~ALCScreenEmitter();

	private:
		ALCColorSamples m_samples;

		QScreen *m_screen;

		QString m_name;
		QRect m_captureArea;
		int m_chunkSize;
		int m_pixelSkip;
		int m_framerateLimit;
		double m_marginProcent;

		QVector <int> *colors[4];

		std::atomic <bool> m_quit;

	public slots:
		void setName(QString name);
		void setCaptureArea(QRect);
		void setChunkSize(int);
		void setPixelSkip(int);
		void setFramerateLimit(int);
		void setQuitState(bool);
		void setMarginProcent(double);

		QRect area();
		int chunk();
		int pixelSkip();
		int framerateLimit();
		double marginProcent();

		QString name();

	public:
		bool configure();

	protected:
		void run();
		void init();
		void done();

	};

}

#endif // ALCSCREENEMITTER_H
