#pragma once

#include <QObject>
#include <QImage>
#include <QTimer>

#include "emitters/alc-emitter.h"

namespace Emitters {

	class ALCImageEmitter : public QObject, public ALCEmitter {
		Q_OBJECT
	private:
		ALCColorSamples m_samples;
		QImage m_image;
		QString m_file;
		bool m_loaded;
		QTimer *m_timer;

	public:
		explicit ALCImageEmitter(QObject *parent = 0);
		~ALCImageEmitter();

		void fromFile(QString file);
		QString file();
		QString open();

		bool isLoaded();

		void pushState();
		void init();
		bool configure();
	};

}
