#include <gtest/gtest.h>

#include <core/functionals/captures/x11-helper.h>

TEST(X11Tests, isOpen) {
	Functional::Helper::X11Helper handle;
	EXPECT_TRUE(handle.is_open());
}

TEST(X11Tests, frameCaptured) {
	Functional::Helper::X11Helper handle;
	EXPECT_TRUE(handle.capture(0, 0, 320, 240));
	EXPECT_NE(handle.data(), nullptr);
}
