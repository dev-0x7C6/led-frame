#include <components/painters/wave-paint-engine.h>

#include <QResizeEvent>
#include <QtMath>
#include <QWidget>

using namespace Widget;

WavePaintEngine::WavePaintEngine(QWidget *parent)
	: m_animationEnabled(false),
	  m_shift(0),
	  m_parent(parent) {
	m_waveGradientModel.addPoint(0.000, QColor::fromRgbF(0.000, 0.000, 0.000), false);
	m_waveGradientModel.addPoint(0.200, QColor::fromRgbF(0.000, 0.000, 0.000), true);
	m_waveGradientModel.addPoint(0.600, QColor::fromRgbF(0.100, 0.100, 0.100), true);
	m_waveGradientModel.addPoint(0.820, QColor::fromRgbF(0.500, 0.500, 0.500), true);
	m_waveGradientModel.addPoint(0.825, QColor::fromRgbF(1.000, 1.000, 1.000), true);
	m_waveGradientModel.addPoint(0.850, QColor::fromRgbF(0.150, 0.150, 0.150), true);
	m_waveGradientModel.addPoint(1.000, QColor::fromRgbF(0.000, 0.000, 0.000), false);
	m_waveGradientModel.setSinMultipler(0.015);
	QObject::connect(&m_reflesh, &QTimer::timeout, [this]() {
		update();
	});
}

WaveGradientModel::WaveGradientModel() {
	clear();
}

void WaveGradientModel::addPoint(qreal point, const QColor &color, bool wavy) {
	WaveGradientPoint p;
	p.point = point;
	p.wavy = wavy;
	p.color = color;
	m_points << p;
}

void WaveGradientModel::clear() {
	m_points.clear();
	m_sinMultipler = 0.04;
}

QList<WaveGradientPoint> &WaveGradientModel::points() {
	return m_points;
}

qreal WaveGradientModel::sinMultipler() const {
	return m_sinMultipler;
}

void WaveGradientModel::setSinMultipler(qreal sin) {
	m_sinMultipler = sin;
}

void WavePaintEngine::paint(const QRect &rect, QPainter &painter) {
	if (!m_animationEnabled)
		return;

	painter.save();
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::NoPen);
	painter.drawPixmap(rect, m_pixmap, {m_shift, 0, rect.width(), m_pixmap.height()});
	painter.restore();
}

void WavePaintEngine::resize(const QSize &size) {
	if (!m_animationEnabled)
		return;

	if (m_pixmap.width() != size.width() * 4 || m_pixmap.height() != size.height()) {
		m_pixmap = { size.width() * 4, size.height() };
	}

	QPainter painter(&m_pixmap);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
	double counter = 0;
	double sample = size.width() / 360;
	double diff = size.width() / sample;

	if (sample <= 0)
		return;

	double hue = 0;

	for (double i = 0; i < m_pixmap.width(); i += sample) {
		double wave = qSin(counter += (M_PI) / diff) * m_waveGradientModel.sinMultipler();
		QLinearGradient gradient(0, 0, 0, size.height());
		hue = i / m_pixmap.width() * 2;

		if (hue > 1.0)
			hue = hue - 1.0;

		for (const WaveGradientPoint &point : m_waveGradientModel.points()) {
			auto color = point.color;
			color = QColor::fromHslF(hue, 1, color.redF() / 2);
			gradient.setColorAt(point.point - (point.wavy ? wave : 0), color);
		}

		painter.fillRect(QRectF(i, 0, i + sample, m_pixmap.height()), gradient);
	}
}

#include <QtDebug>

void WavePaintEngine::update() {
	if (!m_animationEnabled)
		return;

	m_shift += m_parent->width() / 200;

	for (uint32_t i = 0; i < Container::ColorScanlineContainer::linesize(); ++i) {
		double step = m_parent->width() / 64;
		double hue = static_cast<double>(m_shift + (step * i)) / m_pixmap.width() * 2;

		if (hue > 1.0)
			hue = hue - 1.0;

		auto color = QColor::fromHslF(hue, 1, 0.5).rgb();
		m_scanline.data(Enum::Position::Top)[i] = color;
		m_scanline.data(Enum::Position::Bottom)[63 - i] = color;

		if (i == 0)
			m_scanline.fill(Enum::Position::Left, color);

		if (i == Container::ColorScanlineContainer::linesize() - 1)
			m_scanline.fill(Enum::Position::Right, color);
	}

	if (m_shift > m_pixmap.width() / 2)
		m_shift = 0;

	m_parent->update();
}

const Container::ColorScanlineContainer &WavePaintEngine::scanline() const {
	return m_scanline;
}

bool WavePaintEngine::animationEnabled() const {
	return m_animationEnabled;
}

void WavePaintEngine::setAnimationEnabled(bool animationEnabled) {
	if (m_animationEnabled == false && animationEnabled == true) {
		m_animationEnabled = animationEnabled;
		resize(m_parent->size());
		update();

		if (!m_reflesh.isActive())
			m_reflesh.start(1000.0 / 24.0);
	} else
		m_animationEnabled = animationEnabled;

	if (m_reflesh.isActive() && m_animationEnabled == false)
		m_reflesh.stop();
}

