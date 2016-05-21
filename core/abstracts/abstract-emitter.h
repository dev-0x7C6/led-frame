#pragma once

#include <core/interfaces/iemitter.h>

#include <atomic>
#include <list>
#include <memory>
#include <mutex>

namespace Abstract {

class AbstractEmitter : public Interface::IEmitter {
public:
	explicit AbstractEmitter();
	virtual ~AbstractEmitter() = default;

	virtual void commit(const Container::ColorScanlineContainer &scanline) override;
	virtual Container::ColorScanlineContainer data() override;

	virtual QString name() const override;
	virtual void setName(const QString &name) override;

	virtual void connect() override;
	virtual void disconnect() override;
	virtual uint32_t connectionCount() override;

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
