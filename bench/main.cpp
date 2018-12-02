#include <benchmark/benchmark.h>

#include <core/containers/color-scanline-container.h>
#include <core/correctors/factories/corrector-factory.h>
#include <core/functionals/color-averaging-buffer.h>
#include <core/functionals/image-block-processor.h>

#include <vector>
#include <memory>

using namespace Container;
using namespace Enum;
using namespace Functional;

namespace Bench {

class Frame {
public:
	explicit Frame(i32 w, i32 h, i32 bpp)
			: m_bpp(bpp)
			, m_data(w * h * bpp) {
	}

	const color *data() const noexcept { return m_data.data(); }
	auto size() const noexcept { return m_data.size(); }
	auto lenght() const noexcept { return size() / m_bpp; }

	void fill(const color value) noexcept {
		std::fill(m_data.begin(), m_data.end(), value);
	}

private:
	const i32 m_bpp{4};
	std::vector<color> m_data;
};

template <cu32 w, cu32 h, cu32 step = 0>
inline static void image_processor_process(benchmark::State &state) {
	constexpr auto matchColor = 0xffffffffu;

	Frame scene(w, h, 4);
	scene.fill(matchColor);

	ImageBlockProcessor<ColorAveragingBuffer, 16, 24> processor;
	while (state.KeepRunning()) {
		processor.process(scene.data(), w, h, step);
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
		correctors.emplace_back(Factory::make_corrector(type, 0));

	while (state.KeepRunning()) {
		for (const auto &corrector : correctors)
			if (corrector->isEnabled())
				corrector->correct(scanline);
	}
}

static void color_interpolation(benchmark::State &state) {
	factor_t value = 0;

	while (state.KeepRunning()) {
		value += 0.01f;

		if (value >= static_cast<factor_t>(1.0))
			value = 0;

		Scanline result;
		Scanline::interpolate(Scanline(0x00), Scanline(0xFF), value, result);
		benchmark::DoNotOptimize(result);
	}
}

void image_block_processor_process_480p_auto(benchmark::State &state) { image_processor_process<640, 480>(state); }
void image_block_processor_process_720p_auto(benchmark::State &state) { image_processor_process<1280, 720>(state); }
void image_block_processor_process_1080p_auto(benchmark::State &state) { image_processor_process<1920, 1080>(state); }
void image_block_processor_process_4K_auto(benchmark::State &state) { image_processor_process<3840, 2160>(state); }
void image_block_processor_process_8K_auto(benchmark::State &state) { image_processor_process<7680, 4320>(state); }

void image_block_processor_process_480p_fixed(benchmark::State &state) { image_processor_process<640, 480, 1>(state); }
void image_block_processor_process_720p_fixed(benchmark::State &state) { image_processor_process<1280, 720, 1>(state); }
void image_block_processor_process_1080p_fixed(benchmark::State &state) { image_processor_process<1920, 1080, 1>(state); }
void image_block_processor_process_4K_fixed(benchmark::State &state) { image_processor_process<3840, 2160, 1>(state); }
void image_block_processor_process_8K_fixed(benchmark::State &state) { image_processor_process<7680, 4320, 1>(state); }
} // namespace Bench

using namespace Bench;

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
