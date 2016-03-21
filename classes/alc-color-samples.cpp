#include "alc-color-samples.h"
#include <unistd.h>

const int ALCColorSamples::Resolution = 30;

ALCColorSamples::ALCColorSamples() {
	m_samples[0].resize(ALCColorSamples::Resolution);
	m_samples[1].resize(ALCColorSamples::Resolution);
	m_samples[2].resize(ALCColorSamples::Resolution);
	m_samples[3].resize(ALCColorSamples::Resolution);
}

ALCColorSamples::~ALCColorSamples() { }

void ALCColorSamples::set(ALCColorSamples::Position pos, const QVector <int> &colors) {
	memcpy(reinterpret_cast <void *>(m_samples[static_cast<int>(pos)].data()),
	       reinterpret_cast <void *>(const_cast<int *>(colors.data())), colors.size() * sizeof(int));
}

QVector <int> *ALCColorSamples::get(ALCColorSamples::Position pos) {
	return &m_samples[static_cast<int>(pos)];
}

QVector <int> *ALCColorSamples::scaled(ALCColorSamples::Position pos, int size) {
	QVector <int> *result = new QVector <int> (size);
	const QVector <int> &samples = m_samples[static_cast<int>(pos)];
	double step = samples.size() / static_cast <double>(size);

	for (int i = 0; i < size; ++i)
		(*result)[i] = samples[i * step];

	(*result)[0] = samples.first();
	(*result)[size - 1] = samples.last();
	return result;
}

QVector <int> *ALCColorSamples::pscaled(ALCColorSamples::Position pos, int size) {
	QVector <int> *result = new QVector <int> (size);
	const QVector <int> &samples = m_samples[static_cast<int>(pos)];
	double step = samples.size() / static_cast <double>(size);
	Q_UNUSED(step);
	return result;
}

quint16 ALCColorSamples::scale() {
	return ALCColorSamples::Resolution;
}

void ALCColorSamples::copy(const ALCColorSamples &ref) {
	for (int i = 0; i < static_cast<int>(Position::Last); ++i)
		set(static_cast<ALCColorSamples::Position>(i), ref.m_samples[i]);
}
