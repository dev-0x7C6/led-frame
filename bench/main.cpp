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

/*
============================================================= 2019-04-15 02:27:36
Run on (4 X 4300 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 6144K (x1)
---------------------------------------------------------------------------------
Benchmark                                          Time           CPU Iterations
---------------------------------------------------------------------------------
color_interpolation                              264 ns        263 ns    2657952
color_correction                                  10 ns         10 ns   70387041
image_block_processor_process_480p_auto         3797 ns       3794 ns     184506
image_block_processor_process_720p_auto         5751 ns       5746 ns     121820
image_block_processor_process_1080p_auto        7985 ns       7978 ns      87710
image_block_processor_process_4K_auto          17184 ns      17168 ns      40704
image_block_processor_process_8K_auto          39934 ns      39892 ns      17514
image_block_processor_process_480p_fixed        9096 ns       9086 ns      77028
image_block_processor_process_720p_fixed       21862 ns      21820 ns      32052
image_block_processor_process_1080p_fixed      48248 ns      48186 ns      14521
image_block_processor_process_4K_fixed        306008 ns     304576 ns       2298
image_block_processor_process_8K_fixed       1408527 ns    1401088 ns        500


============================================================= 2018-12-02 19:38:07
Run on (4 X 4300 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 6144K (x1)
---------------------------------------------------------------------------------
Benchmark                                          Time           CPU Iterations
---------------------------------------------------------------------------------
color_interpolation                              407 ns        407 ns    1719033
color_correction                                  10 ns         10 ns   71524940
image_block_processor_process_480p_auto         4534 ns       4531 ns     154550
image_block_processor_process_720p_auto         6452 ns       6446 ns     108601
image_block_processor_process_1080p_auto        8810 ns       8800 ns      79501
image_block_processor_process_4K_auto          17964 ns      17943 ns      39041
image_block_processor_process_8K_auto          40865 ns      40802 ns      17222
image_block_processor_process_480p_fixed        9333 ns       9324 ns      75076
image_block_processor_process_720p_fixed       22053 ns      22028 ns      31773
image_block_processor_process_1080p_fixed      47948 ns      47865 ns      14623
image_block_processor_process_4K_fixed        355167 ns     353468 ns       2010
image_block_processor_process_8K_fixed       1387108 ns    1379685 ns        508

*/

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

template <u32 w, u32 h, u32 step = 0>
inline static void image_processor_process(benchmark::State &state) {
	constexpr auto matchColor = 0xffffffffu;

	Frame scene(w, h, 4);
	scene.fill(matchColor);

	while (state.KeepRunning()) {
		auto ret = ImageBlockProcessor<ColorAveragingBuffer<Enum::ColorFormat::RGB>, 16, 24>::process(scene.data(), w, h, step);
		benchmark::DoNotOptimize(ret);
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

		auto ret = interpolate(Scanline(0x00), Scanline(0xFF), value);
		benchmark::DoNotOptimize(ret);
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
