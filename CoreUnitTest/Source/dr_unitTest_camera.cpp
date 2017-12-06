#include <dr_core_prerequisites.h>
#include <gtest\gtest.h>

#include <dr_camera.h>
#include <dr_quaternion.h>

TEST(camera, defaultConstructor)
{
	driderSDK::Camera* pCamera;
	pCamera = new driderSDK::Camera;
	EXPECT_FALSE(nullptr == pCamera);
	delete pCamera;
}

TEST(camera, constructor)
{
	driderSDK::Camera Cam("test", 
												driderSDK::Vector3D(0, 0, 0), 
												driderSDK::Vector3D(0, 0, 1), 
												driderSDK::Viewport{ 0,0,900,600,0,1 }, 
												90, 
												0.1, 
												1000);
	EXPECT_TRUE(Cam.getName() == "test");
}

TEST(camera, destructor)
{
	driderSDK::Camera Cam("test", 
												driderSDK::Vector3D(0, 0, 0), 
												driderSDK::Vector3D(0, 0, 1), 
												driderSDK::Viewport{ 0,0,900,600,0,1 }, 
												90, 
												0.1, 
												1000);
	driderSDK::Camera* pCam = nullptr;
	pCam = &Cam;
	EXPECT_TRUE(pCam->getName() == "test");
}

TEST(camera, update)
{
	driderSDK::Camera Cam("test", 
												driderSDK::Vector3D(-1, 0, 0), 
												driderSDK::Vector3D(0, 0, 0), 
												driderSDK::Viewport{ 0,0,900,600,0,1 }, 
												90, 
												0.1, 
												1000);
	Cam.move(10,0);
	driderSDK::Matrix4x4 vp = Cam.m_vp;
	Cam.update(1);
	EXPECT_TRUE(vp != Cam.m_vp);
}

TEST(camera, move)
{
	driderSDK::Camera Cam("test", 
												driderSDK::Vector3D(-1, 0, 0), 
												driderSDK::Vector3D(0, 0, 0), 
												driderSDK::Viewport{ 0,0,900,600,0,1 }, 
												90, 
												0.1, 
												1000);
	Cam.move(-10, 0);
	Cam.update(1);
	EXPECT_FLOAT_EQ(Cam.m_pos.x, -11.f);
	EXPECT_FLOAT_EQ(Cam.m_pos.y, 0.f);
	EXPECT_FLOAT_EQ(Cam.m_pos.z, 0.f);
}

TEST(camera, pan)
{
	driderSDK::Camera Cam("test", 
												driderSDK::Vector3D(-1, 0, 0), 
												driderSDK::Vector3D(0, 0, 0), 
												driderSDK::Viewport{ 0,0,900,600,0,1 }, 
												90, 
												0.1, 
												1000);
	Cam.pan(10, 0);
	Cam.update(1);
	EXPECT_FLOAT_EQ(Cam.m_pos.x, 9.f);
	EXPECT_FLOAT_EQ(Cam.m_pos.y, 0.f);
	EXPECT_FLOAT_EQ(Cam.m_pos.z, 0.f);

	EXPECT_FLOAT_EQ(Cam.m_target.x, 10.f);
	EXPECT_FLOAT_EQ(Cam.m_target.y, 0.f);
	EXPECT_FLOAT_EQ(Cam.m_target.z, 0.f);
}

TEST(camera, createProyection)
{
	driderSDK::Camera Cam("test",
												driderSDK::Vector3D(0, 0, 0),
												driderSDK::Vector3D(0, 0, 1),
												driderSDK::Viewport{ 0,0,900,600,0,1 },
												90,
												0.1,
												1000);
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
	driderSDK::Camera Cam("test", 
												driderSDK::Vector3D(0, 0, 0), 
												driderSDK::Vector3D(0, 0, 1), 
												driderSDK::Viewport{ 0,0,900,600,0,1 }, 
												90, 
												0.1, 
												1000);
	driderSDK::Viewport Viewport{ 0,0,1024,720,0,1 };
	Cam.setViewport(Viewport);
	EXPECT_EQ(Cam.m_viewport.width, 1024);
}

TEST(camera, getName)
{
	driderSDK::Camera Cam("test", 
												driderSDK::Vector3D(0, 0, 0), 
												driderSDK::Vector3D(0, 0, 1), 
												driderSDK::Viewport{ 0,0,900,600,0,1 }, 
												90, 
												0.1, 
												1000);
	EXPECT_TRUE(Cam.getName() == "test");
}

TEST(camera, rotate)
{
	driderSDK::Camera Cam("test",
												driderSDK::Vector3D(0, 0, 0),
												driderSDK::Vector3D(1, 0, 0),
												driderSDK::Viewport{ 0,0,900,600,0,1 },
												90,
												0.1,
												1000);
	driderSDK::Quaternion rot(0.f, 0.7071203316249954f, 0.f, 0.7071203316249954f);
	Cam.rotate(rot);
	Cam.update(1);

	EXPECT_FLOAT_EQ(Cam.m_look.x, 0);
	EXPECT_FLOAT_EQ(Cam.m_look.y, 0);
	EXPECT_FLOAT_EQ(Cam.m_look.z, -1.f);

}

TEST(camera, orbit)
{
	driderSDK::Camera Cam("test",
												driderSDK::Vector3D(1, 0, 0),
												driderSDK::Vector3D(0, 0, 0),
												driderSDK::Viewport{ 0,0,900,600,0,1 },
												90,
												0.1,
												1000);
	Cam.orbit(0, 45);
	Cam.update(1);
	EXPECT_FLOAT_EQ(Cam.m_pos.x, 0);
	EXPECT_FLOAT_EQ(Cam.m_pos.y, 1.f);
	EXPECT_FLOAT_EQ(Cam.m_pos.z, 0);
}

TEST(camera, getVP)
{
	driderSDK::Vector3D pos(0, 0, 0);
	driderSDK::Camera Cam("test",
												pos,
												driderSDK::Vector3D(1, 0, 0),
												driderSDK::Viewport{ 0,0,1024,720,0,1 },
												90,
												10,
												1000);
	Cam.update(1);
	driderSDK::Matrix4x4 VP = Cam.m_vp;
	EXPECT_TRUE(VP == Cam.getVP());
}