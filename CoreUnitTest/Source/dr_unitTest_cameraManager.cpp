#include <dr_core_prerequisites.h>
#include <gtest\gtest.h>

#include <dr_camera_manager.h>
#include <dr_camera.h>
#include <dr_vector3d.h>

TEST(CameraManager, constructor)
{
	driderSDK::CameraManager* pManager;
	pManager = new driderSDK::CameraManager;
	EXPECT_FALSE(nullptr == pManager);
	delete pManager;
}

TEST(CameraManager, destructor)
{
	EXPECT_TRUE(false);
}

TEST(CameraManager, createCamera)
{
	driderSDK::CameraManager pManager;
	pManager.createCamera("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	pManager.setActiveCamera("test");
	EXPECT_EQ(pManager.getActiveCamera().getName(), "test");
}

TEST(CameraManager, deleteCamera)
{
	driderSDK::CameraManager pManager;
	pManager.createCamera("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	pManager.setActiveCamera("test");
	pManager.deleteCamera("test");
	EXPECT_TRUE(&pManager.getActiveCamera() == nullptr);
}

TEST(CameraManager, setViewportToCamera)
{
	driderSDK::CameraManager pManager;
	driderSDK::Viewport viewport{0,0,1024,720,0,1};
	pManager.createCamera("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	pManager.setActiveCamera("test");
	pManager.setViewportToCamera("test", viewport);
	EXPECT_EQ(pManager.getActiveCamera().m_viewport.width, 1024);
}

TEST(CameraManager, getActiveCamera)
{
	driderSDK::CameraManager pManager;
	pManager.createCamera("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	pManager.setActiveCamera("test");
	EXPECT_EQ(pManager.getActiveCamera().getName(), "test");
}

TEST(CameraManager, setActiveCamera)
{
	driderSDK::CameraManager pManager;
	pManager.createCamera("test", driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	pManager.setActiveCamera("test");
	EXPECT_EQ(pManager.getActiveCamera().getName(), "test");
}

