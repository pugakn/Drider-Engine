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
	driderSDK::Camera Cam(_T("test"),
                        driderSDK::Viewport{ 0,0,900,600,0,1 });
	EXPECT_TRUE(Cam.getName() == _T("test"));
}

TEST(camera, destructor)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	driderSDK::Camera* pCam = &Cam;
	EXPECT_TRUE(pCam->getName() == _T("test"));
}

TEST(camera, update)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	Cam.move(10.0f, 0.0f, 0.0f, false);
	driderSDK::Matrix4x4 vp = Cam.getVP();
	Cam.update();
	EXPECT_TRUE(vp != Cam.getVP());
}

TEST(camera, move)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	Cam.move(-10, 0, 0, false);
	Cam.update();
	EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().x, -11.f);
	EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().y, 0.f);
	EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().z, 0.f);
}

TEST(camera, pan)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	Cam.pan(10, 0, 0.0f, false);
	Cam.update();
	EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().x, 9.f);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().y, 0.f);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().z, 0.f);

	EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().x, 10.f);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().y, 0.f);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().z, 0.f);
}

TEST(camera, createProyection)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	Cam.setViewport(driderSDK::Viewport{ 0.f,0.f, 1, 1,0.f,1.f });
	Cam.createProyection(60*driderSDK::Math::RADIAN_TO_DEGREE,10,1000);



	EXPECT_FLOAT_EQ(Cam.getView().data[0][0], 0.15611996f);
	EXPECT_FLOAT_EQ(Cam.getView().data[1][1], -0.20763955f);
	EXPECT_FLOAT_EQ(Cam.getView().data[2][2], -1.010101f);
	EXPECT_FLOAT_EQ(Cam.getView().data[2][3], -1.f);
	EXPECT_FLOAT_EQ(Cam.getView().data[3][2], -10.10101f);

	/*temp.ProjectionFov(60, 1.33f, 10, 1000);
	checkValuesMatrix(temp,
										0.15611996f, 0, 0, 0,
										0, -0.20763955f, 0, 0,
										0, 0, -1.010101f, -1,
										0, 0, -10.10101, 0);*/
}

TEST(camera, setViewport)
{
  driderSDK::Viewport Viewport1{ 0, 0, 1024, 720, 0, 1 };
  driderSDK::Viewport Viewport2{ 0, 0, 2048, 1440, 0, 1 };
	driderSDK::Camera Cam1(_T("test1"),
												Viewport1);
  driderSDK::Camera Cam2(_T("test2"),
												Viewport1);
	EXPECT_TRUE(false);
}

TEST(camera, getName)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	EXPECT_TRUE(_T("test") == Cam.getName());
}

TEST(camera, rotate)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	driderSDK::Quaternion rot(0.f, 0.7071203316249954f, 0.f, 0.7071203316249954f);
	Cam.rotate(rot);
	Cam.update();

	EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().x, 0);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().y, 0);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().z, -1.f);
}

TEST(camera, orbit)
{
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,900,600,0,1 });
	Cam.orbit(0, 45);
	Cam.update();
	EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().x, 0);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().y, 1.f);
  EXPECT_FLOAT_EQ(Cam.getTransform().getPosition().z, 0);
}

TEST(camera, getVP)
{
	driderSDK::Vector3D pos(0, 0, 0);
	driderSDK::Camera Cam(_T("test"),
												driderSDK::Viewport{ 0,0,1024,720,0,1 });
	Cam.update();
	driderSDK::Matrix4x4 VP = Cam.getVP();
	EXPECT_TRUE(VP == Cam.getVP());
}