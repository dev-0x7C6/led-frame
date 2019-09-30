#include "camera-emitter.h"

#include <core/emitters/concretes/camera-emitter.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>
#include <externals/common/logger/logger.hpp>
#include <externals/common/video/resolution.hpp>

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

using framerate = int;

using namespace std::chrono_literals;

using namespace Container;
using namespace Emitter::Concrete;
using namespace Enum;
using namespace Container;
using namespace Functional;

namespace {

class raii_video_frame_map {
public:
	raii_video_frame_map(QVideoFrame &frame, const QAbstractVideoBuffer::MapMode mode)
			: m_frame(frame)
			, m_mapped(m_frame.map(mode)) {
		m_frame.map(mode);
	}

	~raii_video_frame_map() {
		if (m_mapped)
			m_frame.unmap();
	}

private:
	QVideoFrame &m_frame;
	bool m_mapped{false};
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

		raii_video_frame_map _(frame, QAbstractVideoBuffer::MapMode::ReadOnly);
		m_update(ImageBlockProcessor<ColorAveragingBuffer, 9, 16>::process(reinterpret_cast<const color *>(frame.bits()), frame.width(), frame.height()));
		return true;
	}

	std::function<void(const Scanline &)> m_update;
};

class ProbeCamera {
public:
	ProbeCamera(const QCameraInfo &info) {
		QCamera camera(info);
		camera.load();
		QMediaRecorder recorder(&camera);

		for (auto &&size : recorder.supportedResolutions())
			m_resolutions.emplace_back(size.width(), size.height());

		for (auto &&rate : recorder.supportedFrameRates())
			m_framerates.emplace_back(rate);

		std::sort(m_resolutions.begin(), m_resolutions.end(), [](const resolution &lhs, const resolution &rhs) {
			const auto lhs_rank = static_cast<int>(lhs.aspect());
			const auto rhs_rank = static_cast<int>(rhs.aspect());
			return lhs_rank < rhs_rank && lhs < rhs;
		});

		std::sort(m_framerates.begin(), m_framerates.end());
	}

	const auto &resolutions() const noexcept { return m_resolutions; }
	const auto &framerates() const noexcept { return m_framerates; }

	bool isValid() const noexcept {
		return !m_resolutions.empty() && !m_framerates.empty();
	}

private:
	std::vector<resolution> m_resolutions;
	std::vector<framerate> m_framerates;
};
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
				ProbeCamera probe(m_info);

				auto &&available_resolutions = probe.resolutions();
				auto &&available_framerates = probe.framerates();

				if (!probe.isValid()) {
					logger<filter>::error(module, "unable to probe camera details");
					std::this_thread::sleep_for(1s);
					error_count++;
					continue;
				}

				handle = std::make_unique<QCamera>(m_info);

				QObject::connect(handle.get(), &QCamera::statusChanged, [](auto &&value) {
					logger<filter>::debug(module, "qt: camera status changed: ", static_cast<int>(value));
				});

				QObject::connect(handle.get(), &QCamera::stateChanged, [](auto &&value) {
					logger<filter>::debug(module, "qt: camera state changed: ", static_cast<int>(value));
				});

				QObject::connect(handle.get(), qOverload<QCamera::Error>(&QCamera::error), [](auto &&value) {
					logger<filter>::debug(module, "qt: camera error id: ", static_cast<int>(value));
				});

				QObject::connect(handle.get(), &QCamera::captureModeChanged, [](auto &&value) {
					logger<filter>::debug(module, "qt: camera capture mode changed: ", static_cast<int>(value));
				});

				const auto prefered_resolution = available_resolutions.front();

				logger<filter>::debug(module, "available resolutions:");
				for (auto &&res : available_resolutions) {
					logger<filter>::debug(module, "  ", res.width(), "x", res.height());
				}

				logger<filter>::debug(module, "available framerats:");
				for (auto &&fps : available_framerates) {
					logger<filter>::debug(module, "  ", fps, " fps");
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

			if (handle && QCamera::Status::UnavailableStatus == handle->status()) {
				logger<filter>::error(module, "camera unavailable, disconnected ?");
				handle.reset(nullptr);
				error_count++;
				continue;
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
