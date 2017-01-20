#include <benchmark/benchmark.h>

#include <core/functionals/image-block-processor.h>
#include <core/functionals/color-averaging-buffer.h>

using namespace Functional;

class ColorScene {
public:
	explicit ColorScene(cu32 w, cu32 h)
			: m_w(w)
			, m_h(h) {
		m_size = m_w * m_h * m_bpp;
		m_data = reinterpret_cast<color *>(std::malloc(m_size));
	}

	~ColorScene() {
		if (m_data != nullptr)
			std::free(reinterpret_cast<void *>(m_data));
	}

	ccolor *data() const noexcept { return m_data; }
	u32 size() const noexcept { return m_size; }
	u32 lenght() const noexcept { return m_size / m_bpp; }

	void fill(color value) noexcept {
		for (u32 i = 0u; i < lenght(); ++i)
			m_data[i] = value;
	}

private:
	cu32 m_w = 0;
	cu32 m_h = 0;
	cu32 m_bpp = 4;
	color *m_data = nullptr;
	u32 m_size = 0;
};

#include <iostream>

template <cu32 w, cu32 h, cu32 step = 0>
inline static void image_processor_process(benchmark::State &state) {
	constexpr auto matchColor = 0xffffffu;

	ColorScene scene(w, h);
	scene.fill(matchColor);

	ImageBlockProcessor<ColorAveragingBuffer, 32, 32> processor;
	while (state.KeepRunning()) {
		processor.process(scene.data(), w, h, step);
	}

	if (processor.top().at(0).count() == 0)
	{
		std::terminate();
	}
}

static void image_block_processor_process_480p_auto(benchmark::State &state) { image_processor_process<640, 480>(state); }
static void image_block_processor_process_720p_auto(benchmark::State &state) { image_processor_process<1280, 720>(state); }
static void image_block_processor_process_1080p_auto(benchmark::State &state) { image_processor_process<1920, 1080>(state); }
static void image_block_processor_process_4K_auto(benchmark::State &state) { image_processor_process<3840, 2160>(state); }
static void image_block_processor_process_8K_auto(benchmark::State &state) { image_processor_process<7680, 4320>(state); }

static void image_block_processor_process_480p_fixed(benchmark::State &state) { image_processor_process<640, 480, 4>(state); }
static void image_block_processor_process_720p_fixed(benchmark::State &state) { image_processor_process<1280, 720, 4>(state); }
static void image_block_processor_process_1080p_fixed(benchmark::State &state) { image_processor_process<1920, 1080, 4>(state); }
static void image_block_processor_process_4K_fixed(benchmark::State &state) { image_processor_process<3840, 2160, 4>(state); }
static void image_block_processor_process_8K_fixed(benchmark::State &state) { image_processor_process<7680, 4320, 4>(state); }

BENCHMARK(image_block_processor_process_480p_auto);
BENCHMARK(image_block_processor_process_720p_auto);
BENCHMARK(image_block_processor_process_1080p_auto);
BENCHMARK(image_block_processor_process_4K_auto);
BENCHMARK(image_block_processor_process_8K_auto);

BENCHMARK(image_block_processor_process_480p_fixed);
BENCHMARK(image_block_processor_process_720p_fixed);
BENCHMARK(image_block_processor_process_1080p_fixed);
BENCHMARK(image_block_processor_process_4K_fixed);
BENCHMARK(image_block_processor_process_8K_fixed);

BENCHMARK_MAIN();
