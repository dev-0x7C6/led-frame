#include "camera-emitter.h"

#include <core/emitters/concretes/camera-emitter.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>
#include <externals/common/logger/logger.hpp>

#include <QAbstractVideoSurface>
#include <QCamera>
#include <QCameraInfo>
#include <QEventLoop>
#include <QMediaRecorder>
#include <QThread>

#include <thread>
#include <chrono>

namespace {
constexpr auto module = "[camera_capture]: ";
constexpr auto filter = error_class::debug;
} // namespace

using namespace std::chrono_literals;

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;
using namespace Container;
using namespace Functional;

namespace {

class resolution {
public:
	enum class aspect_ratio {
		_16_9,
		_16_10,
		_4_3,
		other
	};

	constexpr resolution(int w, int h)
			: m_w(w)
			, m_h(h) {}
	constexpr auto width() const noexcept { return m_w; }
	constexpr auto height() const noexcept { return m_h; }

	constexpr auto aspect() const noexcept {
		if ((m_w * 9) / 16 == m_h)
			return aspect_ratio::_16_9;

		if ((m_w * 3) / 4 == m_h)
			return aspect_ratio::_4_3;

		if ((m_w * 10) / 16 == m_h)
			return aspect_ratio::_16_10;

		return aspect_ratio::other;
	}

	constexpr auto is_aspect_16_9() const noexcept {
		return aspect_ratio::_16_9 == aspect();
	}

	constexpr auto is_aspect_16_10() const noexcept {
		return aspect_ratio::_16_10 == aspect();
	}

	constexpr auto is_aspect_4_3() const noexcept {
		return aspect_ratio::_4_3 == aspect();
	}

	constexpr auto is_valid() const noexcept {
		return (m_w == 0) && (m_h == 0);
	}

	friend constexpr auto operator==(const resolution &lhs, const resolution &rhs) noexcept {
		return lhs.m_w == rhs.m_w && lhs.m_h == rhs.m_h;
	}

	friend constexpr auto operator!=(const resolution &lhs, const resolution &rhs) noexcept {
		return !operator==(lhs, rhs);
	}

	friend constexpr auto operator<(const resolution &lhs, const resolution &rhs) noexcept {
		return (lhs.m_w * lhs.m_h) < (rhs.m_w * rhs.m_h);
	}

private:
	int m_w{};
	int m_h{};
};

template <QVideoFrame::PixelFormat format>
class Capture : public QAbstractVideoSurface {
public:
	Capture(std::function<void(const Scanline &)> &&update)
			: m_update(std::move(update)) {}

private:
	QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType) const final {
		return {format};
	}

	bool present(const QVideoFrame &buffer) final {
		QVideoFrame frame(buffer);

		if (!buffer.isValid())
			return false;

		frame.map(QAbstractVideoBuffer::MapMode::ReadOnly);
		m_update(ImageBlockProcessor<ColorAveragingBuffer, 9, 16>::process(reinterpret_cast<const color *>(frame.bits()), frame.width(), frame.height()));
		return true;
	}

	std::function<void(const Scanline &)> m_update;
};

template <typename Iterator>
auto prepare_prefered_resolutions(Iterator begin, Iterator end) noexcept {
	std::unordered_map<i32, std::vector<resolution>> table;

	std::vector<resolution> resolutions_16_9;
	std::vector<resolution> resolutions_16_10;
	std::vector<resolution> resolutions_4_3;
	std::vector<resolution> resolutions_other;
	std::vector<resolution> resolutions;

	for (; begin != end; ++begin) {
		const auto mode = *begin;
		resolution res(mode.width(), mode.height());
		table[static_cast<i32>(res.aspect())].emplace_back(std::move(res));

		switch (res.aspect()) {
			case resolution::aspect_ratio::_16_9:
				resolutions_16_9.emplace_back(std::move(res));
				break;
			case resolution::aspect_ratio::_16_10:
				resolutions_16_10.emplace_back(std::move(res));
				break;
			case resolution::aspect_ratio::_4_3:
				resolutions_4_3.emplace_back(std::move(res));
				break;
			case resolution::aspect_ratio::other:
				resolutions_other.emplace_back(std::move(res));
				break;
		}
	}

	std::sort(resolutions_16_9.begin(), resolutions_16_9.end());
	std::sort(resolutions_16_10.begin(), resolutions_16_10.end());
	std::sort(resolutions_4_3.begin(), resolutions_4_3.end());
	std::sort(resolutions_other.begin(), resolutions_other.end());
	resolutions.reserve(resolutions_16_9.size() + resolutions_16_10.size() + resolutions_4_3.size() + resolutions_other.size());

	std::move(resolutions_16_9.begin(), resolutions_16_9.end(), std::back_inserter(resolutions));
	std::move(resolutions_16_10.begin(), resolutions_16_10.end(), std::back_inserter(resolutions));
	std::move(resolutions_4_3.begin(), resolutions_4_3.end(), std::back_inserter(resolutions));
	std::move(resolutions_other.begin(), resolutions_other.end(), std::back_inserter(resolutions));

	return resolutions;
}
} // namespace

class CameraWorker : public QThread {
public:
	CameraWorker(IEmitter &emitter, QCameraInfo &&info, std::atomic<bool> &interrupted)
			: m_emitter(emitter)
			, m_info(std::move(info))
			, m_interrupted(interrupted) {
		start();
	}

	~CameraWorker() {
		m_interrupted = true;
		wait();
	}

protected:
	void run() final {
		QEventLoop loop;
		std::unique_ptr<QCamera> handle;

		Capture<QVideoFrame::Format_ARGB32> capture([this](auto &&scanline) { m_emitter.commit(std::forward<decltype(scanline)>(scanline)); });

		const auto process = [&loop]() noexcept {
			loop.processEvents(QEventLoop::AllEvents, 10);
			std::this_thread::sleep_for(1ms); // avoid busy loop
		};

		auto error_count{0};

		while (!m_interrupted) {
			if (error_count > 3) {
				logger<filter>::error(module, "error count exceeded");
				break;
			}

			while (m_emitter.usages() == 0) {
				if (m_interrupted)
					return;

				if (handle)
					handle.reset(nullptr);

				process();
			}

			if (!handle) {
				auto [available_resolutions, available_framerates] = [this]() {
					QCamera camera(m_info);
					camera.load();
					QMediaRecorder rec(&camera);
					return std::make_pair(rec.supportedResolutions(), rec.supportedFrameRates());
				}();

				if (available_resolutions.empty()) {
					logger<filter>::error(module, "no available resolutions");
					std::this_thread::sleep_for(1s);
					error_count++;
					continue;
				}

				if (available_framerates.empty()) {
					logger<filter>::error(module, "no available framerates");
					std::this_thread::sleep_for(1s);
					error_count++;
					continue;
				}

				handle = std::make_unique<QCamera>(m_info);

				std::sort(available_framerates.begin(), available_framerates.end());
				const auto prefered_resolutions = prepare_prefered_resolutions(available_resolutions.begin(), available_resolutions.end());
				const auto prefered_resolution = prefered_resolutions.front();

				logger<filter>::debug(module, "available resolutions:");
				for (auto &&res : prefered_resolutions) {
					logger<filter>::debug("  ", res.width(), "x", res.height());
				}

				logger<filter>::debug(module, "available framerats:");
				for (auto &&fps : available_framerates) {
					logger<filter>::debug("  ", fps, " fps");
				}

				logger<filter>::information(module, "selected resolution: ", prefered_resolution.width(), "x", prefered_resolution.height(), "@", available_framerates.back(), "fps");

				QCameraViewfinderSettings settings;
				settings.setResolution(prefered_resolution.width(), prefered_resolution.height());
				settings.setMinimumFrameRate(available_framerates.front());
				settings.setMaximumFrameRate(available_framerates.back());

				handle->setViewfinderSettings(settings);
				handle->setViewfinder(&capture);
				handle->start();
			}

			if (handle && QCamera::Error::NoError != handle->error()) {
				logger<filter>::error(module, "camera error: ", handle->errorString().toStdString());
				handle.reset(nullptr);
				error_count++;
				continue;
			}

			process();
		}

		logger<filter>::debug(module, "quiting");

		handle.reset(nullptr);
		process();
		m_emitter.invalidate();
	}

private:
	IEmitter &m_emitter;
	QCameraInfo m_info;
	std::atomic<bool> &m_interrupted;
};

CameraEmitter::CameraEmitter(QCameraInfo &&info)
		: m_worker(std::make_unique<CameraWorker>(*this, std::move(info), m_interrupted)) {
}

CameraEmitter::~CameraEmitter() {
	m_interrupted = true;
}
