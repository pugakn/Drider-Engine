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
	driderSDK::CameraManager Manager;
	driderSDK::CameraManager* pManager = nullptr;
	pManager = &Manager;
	EXPECT_TRUE(&pManager->getActiveCamera() == nullptr);
}

TEST(CameraManager, createCamera)
{
	driderSDK::CameraManager Manager;
	Manager.createCamera(_T("test"), driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	Manager.setActiveCamera(_T("test"));
	EXPECT_EQ(Manager.getActiveCamera().getName(), _T("test"));
}

TEST(CameraManager, deleteCamera)
{
	driderSDK::CameraManager Manager;
	Manager.createCamera(_T("test"), driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	Manager.setActiveCamera(_T("test"));
	Manager.deleteCamera(_T("test"));
	EXPECT_TRUE(&Manager.getActiveCamera() == nullptr);
}

TEST(CameraManager, setViewportToCamera)
{
	driderSDK::CameraManager Manager;
	driderSDK::Viewport viewport{0,0,1024,720,0,1};
	Manager.createCamera(_T("test"), driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	Manager.setActiveCamera(_T("test"));
	Manager.setViewportToCamera(_T("test"), viewport);
	EXPECT_EQ(Manager.getActiveCamera().m_viewport.width, 1024);
}

TEST(CameraManager, getActiveCamera)
{
	driderSDK::CameraManager Manager;
	Manager.createCamera(_T("test"), driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	Manager.setActiveCamera(_T("test"));
	EXPECT_EQ(Manager.getActiveCamera().getName(), _T("test"));
}

TEST(CameraManager, setActiveCamera)
{
	driderSDK::CameraManager Manager;
	Manager.createCamera(_T("test"), driderSDK::Vector3D(0, 0, 0), driderSDK::Vector3D(0, 0, 1), driderSDK::Viewport{ 0,0,900,600,0,1 }, 90, 0.1f, 1000);
	Manager.setActiveCamera(_T("test"));
	EXPECT_EQ(Manager.getActiveCamera().getName(), _T("test"));
}

