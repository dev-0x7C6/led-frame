#pragma once

#include <components/widgets/decorated-dialog.h>
#include <core/emitters/abstracts/abstract-emitter.h>

namespace Widget {
namespace Ui {
class AboutDialog;
}

class AboutDialog final : public DecoratedDialog {
	Q_OBJECT
public:
	explicit AboutDialog(QWidget *parent = nullptr);
	virtual ~AboutDialog();

private:
	Ui::AboutDialog *m_ui;
};
}
