#pragma once

#include <core/interfaces/iemitter.h>

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

	private:
		Container::ColorScanlineContainer m_data;
		std::mutex m_mutex;
		QString m_name;
	};

}
