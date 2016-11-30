#pragma once

#include <core/emitters/interfaces/iemitter.h>

#include <atomic>
#include <list>
#include <memory>
#include <mutex>

namespace Emitter {
namespace Abstract {

class AbstractEmitter : public Emitter::Interface::IEmitter {
public:
	explicit AbstractEmitter(int id);
	virtual ~AbstractEmitter() override = default;

	void commit(const Container::ColorScanlineContainer &scanline) override;
	Container::ColorScanlineContainer data() override;

	QString name() const override;
	void setName(const QString &name) override;

	void connect() override;
	void disconnect() override;
	uint32_t connectionCount() override;

protected:
	virtual void onConnect(const uint32_t &count) = 0;
	virtual void onDisconnect(const uint32_t &count) = 0;

private:
	Container::ColorScanlineContainer m_data;
	std::mutex m_mutex;
	QString m_name;
	std::atomic<uint32_t> m_connectionCount;
};
}
}
