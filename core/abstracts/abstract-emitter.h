#pragma once

#include <core/interfaces/iemitter.h>

#include <list>
#include <memory>
#include <mutex>

namespace Abstract {

	class AbstractEmitter : public Interface::IEmitter {
	public:
		explicit AbstractEmitter();
		virtual ~AbstractEmitter();

		virtual void commit(const Container::ColorScanlineContainer &scanline) override;
		virtual Container::ColorScanlineContainer data() override;

	private:
		Container::ColorScanlineContainer m_data;
		std::mutex m_mutex;
	};

}
