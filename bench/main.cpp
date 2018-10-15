#include <benchmark/benchmark.h>

#include <core/containers/color-scanline-container.h>
#include <core/correctors/factories/corrector-factory.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>

#include <vector>
#include <memory>

using namespace Container;
using namespace Corrector;

using namespace Corrector::Factory;
using namespace Enum;
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
	constexpr auto matchColor = 0xffffffffu;

	ColorScene scene(w, h);
	scene.fill(matchColor);

	ImageBlockProcessor<ColorAveragingBuffer, 16, 24> processor;
	while (state.KeepRunning()) {
		processor.process(scene.data(), w, h, step);
	}

	if (processor.output().at(0) == 0) {
		std::cout << std::endl;
	}
}

static void color_correction(benchmark::State &state) {
	Scanline scanline;
	scanline.fill(0xff0000u);

	const auto type_list = {
		CorrectorType::ColorEnhancer,
		CorrectorType::Brightness,
		CorrectorType::RedChannel,
		CorrectorType::GreenChannel,
		CorrectorType::BlueChannel,
		CorrectorType::FlickrEffect};

	std::vector<std::shared_ptr<ICorrector>> correctors;

	for (const auto &type : type_list)
		correctors.emplace_back(Factory::CorrectorFactory::create(type, 0));

	while (state.KeepRunning()) {
		for (const auto &corrector : correctors)
			if (corrector->isEnabled())
				corrector->correct(scanline);
	}
}

static void color_interpolation(benchmark::State &state) {
	factor_t value = 0;

	while (state.KeepRunning()) {
		value += 0.01;

		if (value >= static_cast<factor_t>(1.0))
			value = 0;

		Scanline result;
		Scanline::interpolate(Scanline(0x00), Scanline(0xFF), value, result);
		benchmark::DoNotOptimize(result);
	}
}

static void image_block_processor_process_480p_auto(benchmark::State &state) { image_processor_process<640, 480>(state); }
static void image_block_processor_process_720p_auto(benchmark::State &state) { image_processor_process<1280, 720>(state); }
static void image_block_processor_process_1080p_auto(benchmark::State &state) { image_processor_process<1920, 1080>(state); }
static void image_block_processor_process_4K_auto(benchmark::State &state) { image_processor_process<3840, 2160>(state); }
static void image_block_processor_process_8K_auto(benchmark::State &state) { image_processor_process<7680, 4320>(state); }

static void image_block_processor_process_480p_fixed(benchmark::State &state) { image_processor_process<640, 480, 1>(state); }
static void image_block_processor_process_720p_fixed(benchmark::State &state) { image_processor_process<1280, 720, 1>(state); }
static void image_block_processor_process_1080p_fixed(benchmark::State &state) { image_processor_process<1920, 1080, 1>(state); }
static void image_block_processor_process_4K_fixed(benchmark::State &state) { image_processor_process<3840, 2160, 1>(state); }
static void image_block_processor_process_8K_fixed(benchmark::State &state) { image_processor_process<7680, 4320, 1>(state); }

BENCHMARK(color_interpolation);
BENCHMARK(color_correction);

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

int main(int argc, char **argv) {
	::benchmark::Initialize(&argc, argv);
	::benchmark::RunSpecifiedBenchmarks();
}
