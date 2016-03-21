#include "alc-color-emitter.h"

#include <QColorDialog>
#include <QTime>
#include <QTimer>

namespace Emitters {

	ALCColorEmitter::ALCColorEmitter() :
		QObject(),
		ALCEmitter(ALCEmitter::Type::PlainColor),
		m_color(Qt::white),
		m_timer(new QTimer(this)) {
		srand(QTime::currentTime().msecsSinceStartOfDay());
		connect(m_timer, &QTimer::timeout, this, &ALCColorEmitter::pushState);
		m_timer->setInterval(1000 / 15);
		m_timer->start();
	}

	ALCColorEmitter::~ALCColorEmitter() {}

	void ALCColorEmitter::setColor(const QColor &color) {
		m_color = color;
	}

	const QColor &ALCColorEmitter::color() {
		return m_color;
	}

	void ALCColorEmitter::pushState() {
		QVector <int> samples(ALCColorSamples::Resolution);
		//    int rgb = qRgb(qMin(m_color.red(), 255.0),
		//                   qMin(m_color.green(), 255.0),
		//                   qMin(m_color.blue(), 255.0));
		int rgb = 0;

		for (int i = 0; i < samples.size(); ++i)
			samples[i] = rgb;

		m_samples.set(ALCColorSamples::Position::Bottom, samples);
		m_samples.set(ALCColorSamples::Position::Left, samples);
		m_samples.set(ALCColorSamples::Position::Top, samples);
		m_samples.set(ALCColorSamples::Position::Right, samples);
		setState(m_samples);
	}

	const QColor &ALCColorEmitter::open() {
		QColor color = QColorDialog::getColor(m_color);

		if (color.isValid())
			return (m_color = color);
		else
			return (m_color);
	}

	bool ALCColorEmitter::configure() {
		return open().isValid();
	}

}
