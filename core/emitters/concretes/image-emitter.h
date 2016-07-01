#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QImage>

namespace Emitter {
namespace Concrete {

class ImageEmitter final : public Abstract::AbstractEmitter {
public:
	explicit ImageEmitter();
	explicit ImageEmitter(const QString &filePath);
	virtual ~ImageEmitter() = default;

	virtual Enum::EmitterType type() const override;
	virtual QJsonObject parameters() const override;
	virtual uint32_t framerate() const override { return 1; }

	QRect fragment(int w, int h, const uint32_t &index);

	bool loadFromFile(const QString &filePath);

protected:
	virtual void onConnect(const uint32_t &count) override;
	virtual void onDisconnect(const uint32_t &count) override;

private:
	QImage m_image;
	QString m_filePath;
};
}
}
