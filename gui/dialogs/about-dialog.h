#pragma once

#include <components/widgets/decorated-dialog.h>
#include <core/abstracts/abstract-emitter.h>


namespace Widget {
	namespace Ui {
		class AboutDialog;
	}

	class AboutDialog final
		: public DecoratedDialog
		, public Abstract::AbstractEmitter

	{
		Q_OBJECT
	public:
		explicit AboutDialog(QWidget *parent = nullptr);
		virtual ~AboutDialog();

		virtual QString name() const override;
		virtual Enum::EmitterType type() const override;

	protected:
		virtual void timerEvent(QTimerEvent *event);

	private:
		Ui::AboutDialog *m_ui;
	};
}
