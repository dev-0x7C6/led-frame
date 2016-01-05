#pragma once

#include <QObject>
#include <QList>

#include "correctors/alc-color-correction.h"
#include <core/enums/color-format-enum.h>

class ALCLedStrip {
public:
	enum Source {
		SourceBottom,
		SourceLeft,
		SourceTop,
		SourceRight
	};

	enum Destination {
		DestinationBottom,
		DestinationLeft,
		DestinationTop,
		DestinationRight
	};

private:
	int m_count;
	bool m_clockwise;
	Source m_source;
	Destination m_destination;
	Enum::ColorFormat m_colorFormat;
	double m_brightness;

public:
	void setClockwise(bool);
	bool clockwise() const;

	void setCount(int count);
	int count() const;

	void setColorFormat(Enum::ColorFormat format);
	Enum::ColorFormat colorFormat() const;

	void setSource(Source);
	Source source() const;

	void setDestination(Destination);
	Destination destination() const;

	void setBrightness(double value);
	double brightness();
};

class ALCStripConfiguration {
private:
	QList <ALCLedStrip *> m_strips;

public:
	explicit ALCStripConfiguration();

	void add(ALCLedStrip::Source source, ALCLedStrip::Destination destination,
	         int count, bool clockwise = true, Enum::ColorFormat color = Enum::ColorFormat::RGB, double brightness = 1.0);

	QList <ALCLedStrip *> list();

};
