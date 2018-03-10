#include "dr_camera_manager.h"
#include "dr_camera.h"

namespace driderSDK {

CameraManager::CameraManager() {
}

CameraManager::~CameraManager() {}

void CameraManager::onStartUp() {
  m_dummyCam = std::make_shared<Camera>();
  m_dummyCam->createProyection(60, 0.1f, 4000.f);
  m_activeCam = m_dummyCam;
}


void
CameraManager::createCamera(const TString& cameraName,
														const Vector3D& pos,
														const Vector3D& target,
														const Viewport& viewport,
														float fov,
														float nearPlane,
														float farPlane) {
	
  auto cam = std::make_shared<Camera>(pos, target);
  
  cam->setViewport(viewport);
  
  cam->createProyection(fov, nearPlane, farPlane);

  instance().m_cameras[cameraName] = cam;
}

void
CameraManager::deleteCamera(const TString& cameraName) {
	
  auto& cm = instance();

  auto& cameras = cm.m_cameras;

  auto it = cameras.find(cameraName);
  
  if (it != cameras.end()) {
    if (it->second == cm.m_activeCam) {
      cm.m_activeCam = cm.m_dummyCam;
    }
  }
}

CameraManager::SharedCamera 
CameraManager::getCamera(const TString& cameraName) {
  
  SharedCamera cam;

  auto& cm = instance();

  auto it = cm.m_cameras.find(cameraName);

  if (it != cm.m_cameras.end()) {
    cam = it->second;
  }

  return cam;
}

CameraManager::SharedCamera
CameraManager::getActiveCamera() {
	return instance().m_activeCam;
}

void
CameraManager::setActiveCamera(const TString& cameraName) {

  auto& cm = instance();

	auto it = cm.m_cameras.find(cameraName);

  if (it != cm.m_cameras.end()) {
    cm.m_activeCam = it->second;
  }
}

void 
CameraManager::setActiveCamera(SharedCamera camera) {
  auto& cm = instance();

  for (auto& cam : cm.m_cameras) {
    if (cam.second == camera) {
      cm.m_activeCam = camera;
      break;
    }
  }
}

}