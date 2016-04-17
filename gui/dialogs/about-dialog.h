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
		virtual void timerEvent(QTimerEvent *event) override;
		virtual void onConnect(const uint32_t &count) override;
		virtual void onDisconnect(const uint32_t &count) override;


	private:
		Ui::AboutDialog *m_ui;
	};
}
