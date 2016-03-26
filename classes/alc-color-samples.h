#pragma once

#include <QVector>

class ALCColorSamples {
public:
	static const int Resolution;

	enum class Position {
		Bottom,
		Left,
		Top,
		Right,
		Last
	};

private:
	QVector <int> m_samples[4];

public:
	explicit ALCColorSamples();
	virtual ~ALCColorSamples();

	void set(Position pos, const QVector <int> &colors);
	QVector <int> *get(Position pos);
	QVector <int> *scaled(Position pos, int size);
	QVector <int> *pscaled(Position pos, int size);

	quint16 scale();

	void copy(const ALCColorSamples &ref);
};
