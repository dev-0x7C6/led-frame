#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QThread>
#include <atomic>

namespace Emitter {
namespace Concrete {

class ScreenEmitter final : public QThread, public Abstract::AbstractEmitter {
public:
	explicit ScreenEmitter();
	virtual ~ScreenEmitter();

	bool setCaptureArea(const int screen);

	virtual Enum::EmitterType type() const override;
	virtual QJsonObject parameters() const override;

protected:
	void interrupt();
	virtual void run() override;

	virtual void onConnect(const uint32_t &count) override;
	virtual void onDisconnect(const uint32_t &count) override;

private:
	QRect fragment(int w, int h, const uint32_t &index);

private:
	std::atomic_bool m_interrupted;
	std::atomic<int32_t> m_x;
	std::atomic<int32_t> m_y;
	std::atomic<int32_t> m_w;
	std::atomic<int32_t> m_h;
};
}
}
