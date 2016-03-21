#pragma once

#include <QColor>
#include <QObject>

#include "classes/alc-color-samples.h"
#include "emitters/alc-emitter.h"

class QTimer;

namespace Emitters {

	class ALCColorEmitter: public QObject, public ALCEmitter {
		Q_OBJECT

	public:
		explicit ALCColorEmitter();
		virtual ~ALCColorEmitter();

		void setColor(const QColor &color);
		const QColor &color();

		void pushState();

		const QColor &open();

		virtual bool configure();

	private:
		ALCColorSamples m_samples;
		QColor m_color;
		QTimer *m_timer;

	};

}
