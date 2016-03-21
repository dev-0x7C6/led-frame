#ifndef ALCANIMATIONEMITTER_H
#define ALCANIMATIONEMITTER_H

#include <QColor>
#include <QObject>
#include <QRgb>
#include <QThread>
#include <QVariantAnimation>

#include <atomic>

#include "emitters/alc-emitter.h"
#include "classes/alc-runtime-sync.h"

namespace Emitters {

	class ALCAnimationEmitter : public QThread, public ALCEmitter {
		Q_OBJECT
	public:
		enum class Animation {
			None,
			Glow,
			Rotate,
			Shift
		};

		enum class Effect {
			None,
			Flicker
		};

		explicit ALCAnimationEmitter();
		virtual ~ALCAnimationEmitter();

		bool open();
		bool configure();

		void setup(Animation animation, Effect effect, int cycle = 10000);

		void setFlickerValue(int flicker = 0);
		int flickerValue();

		void setQuitState(bool state = true);

	protected:
		void run();
		void init();
		void done();

	private:
		std::atomic <Animation> m_animation;
		std::atomic <Effect> m_effect;
		std::atomic <quint32> m_rgb;
		std::atomic <quint64> m_flicker;
		std::atomic <bool> m_quit;

		QVariantAnimation m_variantAnimation;

		void process(const QVariant &value);
	};

}

#endif // ALCANIMATIONEMITTER_H
