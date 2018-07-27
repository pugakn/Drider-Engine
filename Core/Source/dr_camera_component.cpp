#include "dr_camera_component.h"

#include <dr_matrix3x3.h>
#include <dr_id_object.h>
#include "dr_camera.h"
#include "dr_camera_manager.h"

namespace driderSDK {

CameraComponent::CameraComponent(GameObject& _gameObject,
                                 SharedCamera _camera) 
  : GameComponent(_gameObject, _T("CameraComponent"))
{
  setCamera(_camera);
}

void 
CameraComponent::setCamera(SharedCamera camera) {
  m_camera = camera;

  if (auto cam = m_camera.lock()) {
      cam->setPosition(m_gameObject.getWorldTransform().getPosition());
  }
}

void 
CameraComponent::setActive() {
  CameraManager::setActiveCamera(m_camera.lock());
}

void
CameraComponent::onCreate() {
}

void 
CameraComponent::onUpdate() {

  if (m_gameObject.changed()) {
    if (auto cam = m_camera.lock()) {
      auto& pos = m_gameObject.getWorldTransform().getPosition();
      auto m3 = Matrix3x3(m_gameObject.getWorldTransform().getRotation());
      auto dir =  m3.transpose() * 
                  Vector3D{0, 0, 1};
      cam->setPosition(pos);
      cam->setTarget(pos + dir * 50.f);
    }
  }
}

void 
CameraComponent::onRender() {
}

void 
CameraComponent::onDestroy() {
}

UInt32
CameraComponent::getClassID() const {
  return CLASS_NAME_ID(CameraComponent);
}

GameComponent* 
CameraComponent::cloneIn(GameObject& _go) {
  return _go.createComponent<CameraComponent>();
}

}