#ifndef LEDCONFIGURATIONWIDGET_H
#define LEDCONFIGURATIONWIDGET_H

#include <QWidget>

namespace Ui {
class LedConfigurationWidget;
}

class LedConfigurationWidget : public QWidget
{
  Q_OBJECT

public:
  explicit LedConfigurationWidget(QWidget *parent = 0);
  ~LedConfigurationWidget();

private:
  Ui::LedConfigurationWidget *ui;
};

#endif // LEDCONFIGURATIONWIDGET_H
