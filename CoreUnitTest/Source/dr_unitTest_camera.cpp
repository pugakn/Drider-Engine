#include <dr_core_prerequisites.h>
#include <gtest\gtest.h>

#include <dr_camera.h>

TEST(camera, defaultConstructor)
{
	driderSDK::Camera* pCamera;
	pCamera = new driderSDK::Camera;
	EXPECT_FALSE(nullptr == pCamera);
	delete pCamera;
}

TEST(camera, constructor)
{
	driderSDK::Camera Cam("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1, 1000);
	EXPECT_TRUE(Cam.getName() == "test");
}

TEST(camera, destructor)
{
	driderSDK::Camera Cam("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1, 1000);
	Cam.~Camera();
	EXPECT_TRUE(Cam.getName() == "test");
}

TEST(camera, update)
{
	EXPECT_TRUE(false);
}

TEST(camera, move)
{
	EXPECT_TRUE(false);
}

TEST(camera, pan)
{
	EXPECT_TRUE(false);
}

TEST(camera, createProyection)
{
	driderSDK::Camera Cam;
	Cam.setViewport(driderSDK::Viewport{ 0.f,0.f,1.33f, 1.f,0.f,1.f });
	Cam.createProyection(60*driderSDK::Math::RADIAN_TO_DEGREE,10,1000);

	EXPECT_FLOAT_EQ(Cam.m_view[0][0], 0.15611996f);
	EXPECT_FLOAT_EQ(Cam.m_view[1][1], -0.20763955f);
	EXPECT_FLOAT_EQ(Cam.m_view[2][2], -1.010101f);
	EXPECT_FLOAT_EQ(Cam.m_view[2][3], -1.f);
	EXPECT_FLOAT_EQ(Cam.m_view[3][2], -10.10101f);

	/*temp.ProjectionFov(60, 1.33f, 10, 1000);
	checkValuesMatrix(temp,
										0.15611996f, 0, 0, 0,
										0, -0.20763955f, 0, 0,
										0, 0, -1.010101f, -1,
										0, 0, -10.10101, 0);*/
}

TEST(camera, setViewport)
{
	driderSDK::Camera Cam("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1, 1000);
	driderSDK::Viewport Viewport{ 0,0,1024,720,0,1 };
	Cam.setViewport(Viewport);
	EXPECT_EQ(Cam.m_viewport.width, 1024);
}

TEST(camera, getName)
{
	driderSDK::Camera Cam("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1, 1000);
	EXPECT_TRUE(Cam.getName() == "test");
}

TEST(camera, rotate)
{
	EXPECT_TRUE(false);
}

TEST(camera, orbit)
{
	EXPECT_TRUE(false);
}