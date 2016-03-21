#ifndef ALCEMITTER_H
#define ALCEMITTER_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QRgb>
#include <QDebug>

#include <atomic>

#include "classes/alc-color-samples.h"

class QTreeWidgetItem;

namespace Emitters {

	class ALCEmitter {
	public:
		enum class Type {
			None,
			ScreenCapture,
			Blackhole,
			Animation,
			PlainColor,
			Image,
			Last
		};

	private:
		QString m_emitterName;
		ALCColorSamples m_safeSamples;
		QTreeWidgetItem *m_treeItem;

	protected:
		Type m_type;
		QMutex m_mutex;

		std::atomic <int> m_connectedCount;

	public:
		explicit ALCEmitter(ALCEmitter::Type type);
		virtual ~ALCEmitter();

		void setEmitterName(const QString &name);
		QString emitterName() const;

		ALCEmitter::Type type() const;
		void setType(const ALCEmitter::Type type);

		virtual void init();
		virtual void done();

		void setState(const ALCColorSamples &samples);
		void state(ALCColorSamples &samples);

		void setTreeItem(QTreeWidgetItem *item);
		QTreeWidgetItem *treeItem();

		virtual bool configure();
		virtual bool rename();
		virtual bool remove();
	};

}

#endif // ALCEMITTER_H
