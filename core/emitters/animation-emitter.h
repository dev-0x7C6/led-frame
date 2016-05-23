#pragma once

#include <core/abstracts/abstract-emitter.h>

#include <QVariantAnimation>

namespace Emitter {

class AnimationEmitter final : public QObject, public Abstract::AbstractEmitter {
public:
	explicit AnimationEmitter(QObject *parent = nullptr);
	virtual ~AnimationEmitter();

	virtual Enum::EmitterType type() const override;
	virtual QJsonObject parameters() const override;

protected:
	void process(const QVariant &value);

	virtual void onConnect(const uint32_t &count) override;
	virtual void onDisconnect(const uint32_t &count) override;

private:
	Container::ColorScanlineContainer m_colors;
	QVariantAnimation m_animation;

};
}
