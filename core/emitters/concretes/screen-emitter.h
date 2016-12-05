#pragma once

#include <core/emitters/abstracts/abstract-emitter.h>

#include <QThread>
#include <atomic>

namespace Emitter {
namespace Concrete {

class ScreenEmitter final : public QThread, public Abstract::AbstractEmitter {
public:
	explicit ScreenEmitter(cint id);
	virtual ~ScreenEmitter();

	bool setCaptureArea(const int screen);

	virtual Enum::EmitterType type() const override;

protected:
	void interrupt();
	virtual void run() override;

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
