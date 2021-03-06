#include <gtest/gtest.h>

#include <iostream>

#include <core/functionals/image-block-processor.h>
#include <core/functionals/color-averaging-buffer.h>

using namespace Functional;

class ColorScene {
public:
	explicit ColorScene(const u32 w, const u32 h)
			: m_w(w)
			, m_h(h) {
		m_size = m_w * m_h * m_bpp;
		m_data = reinterpret_cast<color *>(std::malloc(m_size));
	}

	~ColorScene() {
		if (m_data != nullptr)
			std::free(reinterpret_cast<void *>(m_data));
	}

	const color *data() const noexcept { return m_data; }
	u32 size() const noexcept { return m_size; }
	u32 lenght() const noexcept { return m_size / m_bpp; }

	void fill(color value) noexcept {
		for (u32 i = 0u; i < lenght(); ++i)
			m_data[i] = value;
	}

private:
	const u32 m_w = 0;
	const u32 m_h = 0;
	const u32 m_bpp = 4;
	color *m_data = nullptr;
	u32 m_size = 0;
};

TEST(ImageBlockProcessor, checkingEmptySpace) {
	constexpr auto w = 1920u;
	constexpr auto h = 1080u;
	constexpr auto matchColor = 0xff0000u;

	ColorScene scene(w, h);
	scene.fill(matchColor);

	const auto output = ImageBlockProcessor<ColorAveragingBuffer<Enum::ColorFormat::RGB>, 32, 32>::process(scene.data(), w, h, 4);

	for (u32 i = 0u; i < output.size(); ++i) {
		EXPECT_EQ(output.at(i), 0xff0000u);
	}
}
