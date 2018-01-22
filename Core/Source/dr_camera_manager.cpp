#include "dr_camera_manager.h"
#include "dr_camera.h"

namespace driderSDK {

CameraManager::CameraManager() {
	m_activeCamera = nullptr;
}

CameraManager::~CameraManager() {}

void
CameraManager::createCamera(const TString& cameraName,
														const Vector3D& pos,
														const Vector3D& target,
														const Viewport& viewport,
														float fov,
														float nearPlane,
														float farPlane) {
	m_cameras.push_back(new Camera(cameraName, 
																 pos, 
																 target, 
																 viewport, 
																 fov, 
																 nearPlane,
																 farPlane));
}

void
CameraManager::deleteCamera(const TString& cameraName) {
	for (auto it = m_cameras.begin(); it != m_cameras.end(); ++it) {
		if ((*it)->getName() == cameraName) {
			if (m_activeCamera->getName() == cameraName) {
				m_activeCamera = nullptr;
			}
			delete *it;
			m_cameras.erase(it);
			return;
		}
	}
}

void
CameraManager::setViewportToCamera(const TString& cameraName, const Viewport& viewport) {
	for (auto &it : m_cameras) {
		if (it->getName() == cameraName) {
			it->setViewport(viewport);
			return;
		}
	}
}

Camera&
CameraManager::getActiveCamera() {
	return *m_activeCamera;
}

void
CameraManager::setActiveCamera(const TString& cameraName) {
	for (auto &it : m_cameras) {
		if (it->getName() == cameraName) {
			m_activeCamera = it;
			return;
		}
	}
}

}