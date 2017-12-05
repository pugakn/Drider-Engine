#include <dr_viewport.h>

#include <dr_core_prerequisites.h>
#include <gtest\gtest.h>

TEST(Viewport, create)
{
	driderSDK::Viewport Viewport{ 0.f, 0.f, 1280.f, 720.f, 0.f, 1.f };
	EXPECT_FLOAT_EQ(Viewport.topLeftX, 0.f);
	EXPECT_FLOAT_EQ(Viewport.topLeftY, 0.f);
	EXPECT_FLOAT_EQ(Viewport.width, 1280.f);
	EXPECT_FLOAT_EQ(Viewport.height, 720.f);
	EXPECT_FLOAT_EQ(Viewport.minDepth, 0.f);
	EXPECT_FLOAT_EQ(Viewport.maxDepth, 1.f);
}