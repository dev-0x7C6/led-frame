#include <gui/tray/system-tray-icon.h>

#include <QPainter>
#include <QRadialGradient>

using namespace Tray;

QIcon SystemTrayIcon::generate(double opacity) {
	QPixmap source(":/tray.png");
	QPixmap sheet(22, 22);
	sheet.fill(QColor::fromRgbF(0, 0, 0, 0));
	QPainter painter(&sheet);
	QRadialGradient radialGrad(QPointF(11, 11), 18);
	radialGrad.setColorAt(0, QColor::fromRgb(0x00, 0x95, 0xf8, 0xff));
	radialGrad.setColorAt(0.75, QColor::fromRgb(0x00, 0x95, 0xf8, 0x00));
	painter.setOpacity(opacity);
	painter.setBrush(radialGrad);
	painter.setPen(Qt::NoPen);
	painter.drawRect(0, 0, 22, 22);
	painter.setOpacity(1.0);
	painter.drawPixmap(3, 3, 16, 16, source);

	return {sheet};
}
